; Auto-generated. Do not edit!


(cl:in-package state_indicator_msgs-msg)


;//! \htmlinclude state_indicator.msg.html

(cl:defclass <state_indicator> (roslisp-msg-protocol:ros-message)
  ((state_msg
    :reader state_msg
    :initarg :state_msg
    :type cl:integer
    :initform 0))
)

(cl:defclass state_indicator (<state_indicator>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <state_indicator>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'state_indicator)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name state_indicator_msgs-msg:<state_indicator> is deprecated: use state_indicator_msgs-msg:state_indicator instead.")))

(cl:ensure-generic-function 'state_msg-val :lambda-list '(m))
(cl:defmethod state_msg-val ((m <state_indicator>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader state_indicator_msgs-msg:state_msg-val is deprecated.  Use state_indicator_msgs-msg:state_msg instead.")
  (state_msg m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <state_indicator>) ostream)
  "Serializes a message object of type '<state_indicator>"
  (cl:let* ((signed (cl:slot-value msg 'state_msg)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <state_indicator>) istream)
  "Deserializes a message object of type '<state_indicator>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'state_msg) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<state_indicator>)))
  "Returns string type for a message object of type '<state_indicator>"
  "state_indicator_msgs/state_indicator")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'state_indicator)))
  "Returns string type for a message object of type 'state_indicator"
  "state_indicator_msgs/state_indicator")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<state_indicator>)))
  "Returns md5sum for a message object of type '<state_indicator>"
  "1374ed518637e22ec0ecc44aa5d4eda0")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'state_indicator)))
  "Returns md5sum for a message object of type 'state_indicator"
  "1374ed518637e22ec0ecc44aa5d4eda0")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<state_indicator>)))
  "Returns full string definition for message of type '<state_indicator>"
  (cl:format cl:nil "int32 state_msg~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'state_indicator)))
  "Returns full string definition for message of type 'state_indicator"
  (cl:format cl:nil "int32 state_msg~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <state_indicator>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <state_indicator>))
  "Converts a ROS message object to a list"
  (cl:list 'state_indicator
    (cl:cons ':state_msg (state_msg msg))
))
