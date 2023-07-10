import { useState, useEffect } from 'react';
import { StyleSheet, Text, View, StatusBar, Button } from 'react-native';
import { Amplify, PubSub, Hub, Auth } from 'aws-amplify';
import { AWSIoTProvider, CONNECTION_STATE_CHANGE, ConnectionState } from '@aws-amplify/pubsub';
import { Marker, Polyline } from 'react-native-maps';
import { GlobalStyles } from '../../constants/styles';
import MapView from 'react-native-maps';
import awsmobile from '../../aws-exports';

Amplify.configure(awsmobile);

Auth.currentCredentials().then((info) => {
    console.log("Current identity", info.identityId);
});

Amplify.addPluggable(
    new AWSIoTProvider({
      aws_pubsub_region: 'ap-northeast-2',
      aws_pubsub_endpoint:
        'wss://a2ht7rbdkt6040-ats.iot.ap-northeast-2.amazonaws.com/mqtt'
    })
);

export default function MyMapView({ manageDeviceId }) {
    const [message, setMessage] = useState('');
    const [region, setRegion] = useState({
        latitude: 21.030212,
        longitude: 105.781917,
        latitudeDelta: 0.001922,
        longitudeDelta: 0.001421
    });

    const [startLocation, setStartLocation] = useState(null);
    const [polylineCoords, setPolylineCoords] = useState([]);

    Hub.listen('pubsub', (data) => {
        const { payload } = data;
        if (payload.event === CONNECTION_STATE_CHANGE) {
            const connectionState = payload.data.connectionState;
            console.log(connectionState);
            console.log("Map Connected!")
        }
    });

    const onRegionChange = newRegion => {
        setRegion(newRegion);
    };

    useEffect(() => {
        let subscription;
        PubSub.subscribe(`anhttm8-tracker/${manageDeviceId}/message`).subscribe({    
            next: data => {      
            /*
            Type of message:
            {
            "message": {
              "lat": "21.030332",
              "long": "105.781966"
              }
            }
            */  
                console.log('Message received:', data.value.message);
                setMessage(data.value.message);
            },
            error: error => console.error(error),    
            close: () => console.log('Done'),  
        });

        return () => {
            if (subscription) {
                subscription.unsubscribe();
             }
        };
    }, []);

    useEffect(() => {
        if (message) {
            setRegion({
                latitude: parseFloat(message.lat),
                longitude: parseFloat(message.long),
                latitudeDelta: 0.021922,
                longitudeDelta: 0.021421
            });
    
            const newCoords = [
                ...polylineCoords,
                {
                    latitude: parseFloat(message.lat),
                    longitude: parseFloat(message.long)
                }
            ];
    
            if (startLocation === null) {
                setStartLocation(newCoords[0]);
            }
    
            setPolylineCoords(newCoords);
        }
    }, [message]);

    return (
        <View style={styles.container}>
        {
            message && (
                <View>
                    <Text>Latitude: {message.lat}</Text>
                    <Text>Longitude: {message.long}</Text>
                </View>
            )
        }
        <MapView 
            style = {styles.map}
            region = {region}
            onRegionChange = {onRegionChange}
        >
            {polylineCoords.length > 0 &&
            (
                <Polyline
                    coordinates = {polylineCoords}
                    strokeColor = "#000"
                    strokeWidth = {2}
                />
            )}
            {message && (
                <Marker
                    coordinate = {{ 
                        latitude : parseFloat(message.lat), 
                        longitude : parseFloat(message.long) 
                    }}
                    title = "Real-time tracking"
                    description = "The location of your asset in real-time"
                />
            )}
            {startLocation && (
                <Marker
                    coordinate = {startLocation}
                    title = "Start Location"
                    description = "The location where tracking started"
                />
            )}
        </MapView>
        <View style={styles.button}>
            <Button
                title="Get current location"
                color="tomato"
            />
        </View>
        <View style={styles.location}>
            <Text>Latitude: </Text>
            <Text>Longitude: </Text>
        </View>
        </View>
    );
}

const styles = StyleSheet.create({
    container: {
      flex: 1,
      backgroundColor: '#fff',
      alignItems: 'center',
      justifyContent: 'center',
    },
    map: {
      ...StyleSheet.absoluteFillObject,
      width: '100%',
      height: '100%',
    },
    // create button styles with orange text at the bottom of the screen
    button: {
        position: 'absolute',
        bottom: 20,
        left: 20,
        backgroundColor: '#fff',
        padding: 10,
        borderRadius: 10,
        elevation: 3,
    },
    location: {
        position: 'absolute',
        top: 20,
        left: 20,
        backgroundColor: '#fff',
        padding: 10,
        borderRadius: 10,
        elevation: 3,
    },
});
