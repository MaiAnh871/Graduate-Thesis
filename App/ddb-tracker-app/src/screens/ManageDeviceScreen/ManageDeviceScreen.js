import { useContext, useEffect } from 'react';
import { StyleSheet, View, Button } from 'react-native';

import { DevicesContext } from '../../store/devices-context';
import { GlobalStyles } from '../../constants/styles';
import MyMapView from '../../components/MyMapView/MyMapView';
import { useDispatch } from 'react-redux';
import { addDeviceID } from '../../redux/reducer';

function ManageDeviceScreen({ route, navigation }) {
    const dispatch = useDispatch();
    const devicesCtx = useContext(DevicesContext);
    const manageDeviceId = route.params?.routeDeviceId;
    const manageId = route.params?.routeId;

    function deleteDeviceHandler() {
        devicesCtx.deleteDevice(manageId);
        navigation.goBack();
    }

    useEffect(() => {
        dispatch(addDeviceID(manageDeviceId))
    }, [dispatch])

    return (
        <View style={ styles.container }>
            <MyMapView manageDeviceId={manageDeviceId}/>
        </View>
    );
};
  
export default ManageDeviceScreen;

const styles = StyleSheet.create({
    deleteContainer: {
        marginTop: 60,
        paddingTop: 8,
        borderTopWidth: 2,
        borderTopColor: "white",
        alignItems: 'center'
    },
    container: {
        flex: 1,
        padding: 0,
        backgroundColor: GlobalStyles.colors.tomato
    }
})