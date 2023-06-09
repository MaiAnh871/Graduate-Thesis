/* Amplify Params - DO NOT EDIT
	API_MYTRACKINGAPP_GRAPHQLAPIENDPOINTOUTPUT
	API_MYTRACKINGAPP_GRAPHQLAPIIDOUTPUT
	API_MYTRACKINGAPP_USERTABLE_ARN
	API_MYTRACKINGAPP_USERTABLE_NAME
	AUTH_MYTRACKINGAPP_USERPOOLID
	ENV
	REGION
Amplify Params - DO NOT EDIT */

import { DynamoDB } from 'aws-sdk';
var ddb = new DynamoDB();

export async function handler(event, context) {
    
    let date = new Date();

    if (event.request.userAttributes.sub) {

        let params = {
            Item: {
                'id': {S: event.request.userAttributes.sub},
                '__typename': {S: 'User'},
                'name': {S: event.request.userAttributes.name},
                'email': {S: event.request.userAttributes.email},
                'createdAt': {S: date.toISOString()},
                'updatedAt': {S: date.toISOString()},
            },
            TableName: process.env.API_mytrackingapp_USERTABLE_NAME
        };

        // Call DynamoDB
        try {
            await ddb.putItem(params).promise()
            console.log("Success");
        } catch (err) {
            console.log("Error", err);
        }

        console.log("Success: Everything executed correctly");
        context.done(null, event);

    } else {
        // Nothing to do, the user's email ID is unknown
        console.log("Error: Nothing was written to DynamoDB");
        context.done(null, event);
    }
};
