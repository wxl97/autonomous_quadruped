
"use strict";

let NodeStatus = require('./NodeStatus.js');
let ImageSegmentation = require('./ImageSegmentation.js');
let Latency = require('./Latency.js');
let FlightStateTransition = require('./FlightStateTransition.js');
let FlightCommand = require('./FlightCommand.js');
let WaypointList = require('./WaypointList.js');
let ProcessStatus = require('./ProcessStatus.js');
let Detection = require('./Detection.js');
let JoyDef = require('./JoyDef.js');
let Box = require('./Box.js');
let ControlMessage = require('./ControlMessage.js');
let Keypoint = require('./Keypoint.js');
let NodeList = require('./NodeList.js');
let ImageDetections = require('./ImageDetections.js');
let TelemString = require('./TelemString.js');
let FlightEvent = require('./FlightEvent.js');

module.exports = {
  NodeStatus: NodeStatus,
  ImageSegmentation: ImageSegmentation,
  Latency: Latency,
  FlightStateTransition: FlightStateTransition,
  FlightCommand: FlightCommand,
  WaypointList: WaypointList,
  ProcessStatus: ProcessStatus,
  Detection: Detection,
  JoyDef: JoyDef,
  Box: Box,
  ControlMessage: ControlMessage,
  Keypoint: Keypoint,
  NodeList: NodeList,
  ImageDetections: ImageDetections,
  TelemString: TelemString,
  FlightEvent: FlightEvent,
};
