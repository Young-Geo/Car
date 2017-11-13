; Auto-generated. Do not edit!


(cl:in-package node_motion-msg)


;//! \htmlinclude motion.msg.html

(cl:defclass <motion> (roslisp-msg-protocol:ros-message)
  ((type
    :reader type
    :initarg :type
    :type cl:fixnum
    :initform 0)
   (lf_forward
    :reader lf_forward
    :initarg :lf_forward
    :type cl:integer
    :initform 0)
   (ri_forward
    :reader ri_forward
    :initarg :ri_forward
    :type cl:integer
    :initform 0)
   (lf_back
    :reader lf_back
    :initarg :lf_back
    :type cl:integer
    :initform 0)
   (ri_back
    :reader ri_back
    :initarg :ri_back
    :type cl:integer
    :initform 0))
)

(cl:defclass motion (<motion>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <motion>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'motion)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name node_motion-msg:<motion> is deprecated: use node_motion-msg:motion instead.")))

(cl:ensure-generic-function 'type-val :lambda-list '(m))
(cl:defmethod type-val ((m <motion>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader node_motion-msg:type-val is deprecated.  Use node_motion-msg:type instead.")
  (type m))

(cl:ensure-generic-function 'lf_forward-val :lambda-list '(m))
(cl:defmethod lf_forward-val ((m <motion>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader node_motion-msg:lf_forward-val is deprecated.  Use node_motion-msg:lf_forward instead.")
  (lf_forward m))

(cl:ensure-generic-function 'ri_forward-val :lambda-list '(m))
(cl:defmethod ri_forward-val ((m <motion>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader node_motion-msg:ri_forward-val is deprecated.  Use node_motion-msg:ri_forward instead.")
  (ri_forward m))

(cl:ensure-generic-function 'lf_back-val :lambda-list '(m))
(cl:defmethod lf_back-val ((m <motion>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader node_motion-msg:lf_back-val is deprecated.  Use node_motion-msg:lf_back instead.")
  (lf_back m))

(cl:ensure-generic-function 'ri_back-val :lambda-list '(m))
(cl:defmethod ri_back-val ((m <motion>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader node_motion-msg:ri_back-val is deprecated.  Use node_motion-msg:ri_back instead.")
  (ri_back m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <motion>) ostream)
  "Serializes a message object of type '<motion>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'type)) ostream)
  (cl:let* ((signed (cl:slot-value msg 'lf_forward)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'ri_forward)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'lf_back)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'ri_back)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <motion>) istream)
  "Deserializes a message object of type '<motion>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'lf_forward) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ri_forward) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'lf_back) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ri_back) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<motion>)))
  "Returns string type for a message object of type '<motion>"
  "node_motion/motion")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'motion)))
  "Returns string type for a message object of type 'motion"
  "node_motion/motion")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<motion>)))
  "Returns md5sum for a message object of type '<motion>"
  "618d170492e3c869a2574b3b1e9242c4")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'motion)))
  "Returns md5sum for a message object of type 'motion"
  "618d170492e3c869a2574b3b1e9242c4")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<motion>)))
  "Returns full string definition for message of type '<motion>"
  (cl:format cl:nil "uint16 type~%int32 lf_forward~%int32 ri_forward~%int32 lf_back~%int32 ri_back~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'motion)))
  "Returns full string definition for message of type 'motion"
  (cl:format cl:nil "uint16 type~%int32 lf_forward~%int32 ri_forward~%int32 lf_back~%int32 ri_back~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <motion>))
  (cl:+ 0
     2
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <motion>))
  "Converts a ROS message object to a list"
  (cl:list 'motion
    (cl:cons ':type (type msg))
    (cl:cons ':lf_forward (lf_forward msg))
    (cl:cons ':ri_forward (ri_forward msg))
    (cl:cons ':lf_back (lf_back msg))
    (cl:cons ':ri_back (ri_back msg))
))
