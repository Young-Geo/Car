; Auto-generated. Do not edit!


(cl:in-package node_infrared-msg)


;//! \htmlinclude infr.msg.html

(cl:defclass <infr> (roslisp-msg-protocol:ros-message)
  ((left
    :reader left
    :initarg :left
    :type cl:fixnum
    :initform 0)
   (right
    :reader right
    :initarg :right
    :type cl:fixnum
    :initform 0))
)

(cl:defclass infr (<infr>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <infr>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'infr)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name node_infrared-msg:<infr> is deprecated: use node_infrared-msg:infr instead.")))

(cl:ensure-generic-function 'left-val :lambda-list '(m))
(cl:defmethod left-val ((m <infr>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader node_infrared-msg:left-val is deprecated.  Use node_infrared-msg:left instead.")
  (left m))

(cl:ensure-generic-function 'right-val :lambda-list '(m))
(cl:defmethod right-val ((m <infr>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader node_infrared-msg:right-val is deprecated.  Use node_infrared-msg:right instead.")
  (right m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <infr>) ostream)
  "Serializes a message object of type '<infr>"
  (cl:let* ((signed (cl:slot-value msg 'left)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'right)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <infr>) istream)
  "Deserializes a message object of type '<infr>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'left) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'right) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<infr>)))
  "Returns string type for a message object of type '<infr>"
  "node_infrared/infr")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'infr)))
  "Returns string type for a message object of type 'infr"
  "node_infrared/infr")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<infr>)))
  "Returns md5sum for a message object of type '<infr>"
  "24825b8956c21f4c3dd28a5a4d09322c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'infr)))
  "Returns md5sum for a message object of type 'infr"
  "24825b8956c21f4c3dd28a5a4d09322c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<infr>)))
  "Returns full string definition for message of type '<infr>"
  (cl:format cl:nil "int8 left~%int8 right~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'infr)))
  "Returns full string definition for message of type 'infr"
  (cl:format cl:nil "int8 left~%int8 right~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <infr>))
  (cl:+ 0
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <infr>))
  "Converts a ROS message object to a list"
  (cl:list 'infr
    (cl:cons ':left (left msg))
    (cl:cons ':right (right msg))
))
