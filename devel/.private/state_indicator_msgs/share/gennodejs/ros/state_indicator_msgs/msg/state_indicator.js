// Auto-generated. Do not edit!

// (in-package state_indicator_msgs.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class state_indicator {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.state_msg = null;
    }
    else {
      if (initObj.hasOwnProperty('state_msg')) {
        this.state_msg = initObj.state_msg
      }
      else {
        this.state_msg = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type state_indicator
    // Serialize message field [state_msg]
    bufferOffset = _serializer.int32(obj.state_msg, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type state_indicator
    let len;
    let data = new state_indicator(null);
    // Deserialize message field [state_msg]
    data.state_msg = _deserializer.int32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'state_indicator_msgs/state_indicator';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '1374ed518637e22ec0ecc44aa5d4eda0';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int32 state_msg
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new state_indicator(null);
    if (msg.state_msg !== undefined) {
      resolved.state_msg = msg.state_msg;
    }
    else {
      resolved.state_msg = 0
    }

    return resolved;
    }
};

module.exports = state_indicator;
