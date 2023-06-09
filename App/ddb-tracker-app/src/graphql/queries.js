/* eslint-disable */
// this is an auto generated file. This will be overwritten

export const getUser = /* GraphQL */ `
  query GetUser($id: ID!) {
    getUser(id: $id) {
      id
      username
      email
      name
      devices {
        items {
          id
          deviceIMEI
          createdAt
          updatedAt
          userDevicesId
          owner
        }
        nextToken
      }
      createdAt
      updatedAt
      owner
    }
  }
`;
export const listUsers = /* GraphQL */ `
  query ListUsers(
    $filter: ModelUserFilterInput
    $limit: Int
    $nextToken: String
  ) {
    listUsers(filter: $filter, limit: $limit, nextToken: $nextToken) {
      items {
        id
        username
        email
        name
        devices {
          nextToken
        }
        createdAt
        updatedAt
        owner
      }
      nextToken
    }
  }
`;
export const getDevice = /* GraphQL */ `
  query GetDevice($id: ID!) {
    getDevice(id: $id) {
      id
      deviceIMEI
      user {
        id
        username
        email
        name
        devices {
          nextToken
        }
        createdAt
        updatedAt
        owner
      }
      messages {
        items {
          id
          imei
          timestamp
          latitude
          longitude
          speed
          acceX
          acceY
          acceZ
          cellID
          rsrp
          tilt_alert
          wheelie_alert
          overspeed_alert
          createdAt
          updatedAt
          deviceMessagesId
        }
        nextToken
      }
      createdAt
      updatedAt
      userDevicesId
      owner
    }
  }
`;
export const listDevices = /* GraphQL */ `
  query ListDevices(
    $filter: ModelDeviceFilterInput
    $limit: Int
    $nextToken: String
  ) {
    listDevices(filter: $filter, limit: $limit, nextToken: $nextToken) {
      items {
        id
        deviceIMEI
        user {
          id
          username
          email
          name
          createdAt
          updatedAt
          owner
        }
        messages {
          nextToken
        }
        createdAt
        updatedAt
        userDevicesId
        owner
      }
      nextToken
    }
  }
`;
export const getMessage = /* GraphQL */ `
  query GetMessage($id: ID!) {
    getMessage(id: $id) {
      id
      device {
        id
        deviceIMEI
        user {
          id
          username
          email
          name
          createdAt
          updatedAt
          owner
        }
        messages {
          nextToken
        }
        createdAt
        updatedAt
        userDevicesId
        owner
      }
      imei
      timestamp
      latitude
      longitude
      speed
      acceX
      acceY
      acceZ
      cellID
      rsrp
      tilt_alert
      wheelie_alert
      overspeed_alert
      createdAt
      updatedAt
      deviceMessagesId
    }
  }
`;
export const listMessages = /* GraphQL */ `
  query ListMessages(
    $filter: ModelMessageFilterInput
    $limit: Int
    $nextToken: String
  ) {
    listMessages(filter: $filter, limit: $limit, nextToken: $nextToken) {
      items {
        id
        device {
          id
          deviceIMEI
          createdAt
          updatedAt
          userDevicesId
          owner
        }
        imei
        timestamp
        latitude
        longitude
        speed
        acceX
        acceY
        acceZ
        cellID
        rsrp
        tilt_alert
        wheelie_alert
        overspeed_alert
        createdAt
        updatedAt
        deviceMessagesId
      }
      nextToken
    }
  }
`;
