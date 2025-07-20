;; Auto-generated. Do not edit!


(when (boundp 'state_indicator_msgs::state_indicator)
  (if (not (find-package "STATE_INDICATOR_MSGS"))
    (make-package "STATE_INDICATOR_MSGS"))
  (shadow 'state_indicator (find-package "STATE_INDICATOR_MSGS")))
(unless (find-package "STATE_INDICATOR_MSGS::STATE_INDICATOR")
  (make-package "STATE_INDICATOR_MSGS::STATE_INDICATOR"))

(in-package "ROS")
;;//! \htmlinclude state_indicator.msg.html


(defclass state_indicator_msgs::state_indicator
  :super ros::object
  :slots (_state_msg ))

(defmethod state_indicator_msgs::state_indicator
  (:init
   (&key
    ((:state_msg __state_msg) 0)
    )
   (send-super :init)
   (setq _state_msg (round __state_msg))
   self)
  (:state_msg
   (&optional __state_msg)
   (if __state_msg (setq _state_msg __state_msg)) _state_msg)
  (:serialization-length
   ()
   (+
    ;; int32 _state_msg
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _state_msg
       (write-long _state_msg s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _state_msg
     (setq _state_msg (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get state_indicator_msgs::state_indicator :md5sum-) "1374ed518637e22ec0ecc44aa5d4eda0")
(setf (get state_indicator_msgs::state_indicator :datatype-) "state_indicator_msgs/state_indicator")
(setf (get state_indicator_msgs::state_indicator :definition-)
      "int32 state_msg
")



(provide :state_indicator_msgs/state_indicator "1374ed518637e22ec0ecc44aa5d4eda0")


