; Auto-generated. Do not edit!


(cl:in-package node_ultrasonic-msg)


;//! \htmlinclude ultr.msg.html

(cl:defclass <ultr> (roslisp-msg-protocol:ros-message)
  ((distance
    :reader distance
    :initarg :distance
    :type cl:float
    :initform 0.0))
)

(cl:defclass ultr (<ultr>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ultr>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ultr)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name node_ultrasonic-msg:<ultr> is deprecated: use node_ultrasonic-msg:ultr instead.")))

(cl:ensure-generic-function 'distance-val :lambda-list '(m))
(cl:defmethod distance-val ((m <ultr>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader node_ultrasonic-msg:distance-val is deprecated.  Use node_ultrasonic-msg:distance instead.")
  (distance m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ultr>) ostream)
  "Serializes a message object of type '<ultr>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'distance))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ultr>) istream)
  "Deserializes a message object of type '<ultr>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'distance) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ultr>)))
  "Returns string type for a message object of type '<ultr>"
  "node_ultrasonic/ultr")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ultr)))
  "Returns string type for a message object of type 'ultr"
  "node_ultrasonic/ultr")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ultr>)))
  "Returns md5sum for a message object of type '<ultr>"
  "6e77fb10f0c8b4833ec273aa9ac74459")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ultr)))
  "Returns md5sum for a message object of type 'ultr"
  "6e77fb10f0c8b4833ec273aa9ac74459")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ultr>)))
  "Returns full string definition for message of type '<ultr>"
  (cl:format cl:nil "float32 distance~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ultr)))
  "Returns full string definition for message of type 'ultr"
  (cl:format cl:nil "float32 distance~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ultr>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ultr>))
  "Converts a ROS message object to a list"
  (cl:list 'ultr
    (cl:cons ':distance (distance msg))
))
