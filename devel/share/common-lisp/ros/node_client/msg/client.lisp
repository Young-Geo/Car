; Auto-generated. Do not edit!


(cl:in-package node_client-msg)


;//! \htmlinclude client.msg.html

(cl:defclass <client> (roslisp-msg-protocol:ros-message)
  ((type
    :reader type
    :initarg :type
    :type cl:fixnum
    :initform 0)
   (data
    :reader data
    :initarg :data
    :type cl:string
    :initform ""))
)

(cl:defclass client (<client>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <client>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'client)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name node_client-msg:<client> is deprecated: use node_client-msg:client instead.")))

(cl:ensure-generic-function 'type-val :lambda-list '(m))
(cl:defmethod type-val ((m <client>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader node_client-msg:type-val is deprecated.  Use node_client-msg:type instead.")
  (type m))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <client>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader node_client-msg:data-val is deprecated.  Use node_client-msg:data instead.")
  (data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <client>) ostream)
  "Serializes a message object of type '<client>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'type)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'data))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'data))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <client>) istream)
  "Deserializes a message object of type '<client>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'data) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'data) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<client>)))
  "Returns string type for a message object of type '<client>"
  "node_client/client")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'client)))
  "Returns string type for a message object of type 'client"
  "node_client/client")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<client>)))
  "Returns md5sum for a message object of type '<client>"
  "a37de7fa0c884435aa5ee4c76f9a1011")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'client)))
  "Returns md5sum for a message object of type 'client"
  "a37de7fa0c884435aa5ee4c76f9a1011")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<client>)))
  "Returns full string definition for message of type '<client>"
  (cl:format cl:nil "uint16 type~%string data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'client)))
  "Returns full string definition for message of type 'client"
  (cl:format cl:nil "uint16 type~%string data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <client>))
  (cl:+ 0
     2
     4 (cl:length (cl:slot-value msg 'data))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <client>))
  "Converts a ROS message object to a list"
  (cl:list 'client
    (cl:cons ':type (type msg))
    (cl:cons ':data (data msg))
))
