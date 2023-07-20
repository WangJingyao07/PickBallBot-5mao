; Auto-generated. Do not edit!


(cl:in-package image-msg)


;//! \htmlinclude ping.msg.html

(cl:defclass <ping> (roslisp-msg-protocol:ros-message)
  ((x
    :reader x
    :initarg :x
    :type cl:integer
    :initform 0)
   (y
    :reader y
    :initarg :y
    :type cl:integer
    :initform 0)
   (r
    :reader r
    :initarg :r
    :type cl:fixnum
    :initform 0))
)

(cl:defclass ping (<ping>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ping>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ping)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name image-msg:<ping> is deprecated: use image-msg:ping instead.")))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <ping>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader image-msg:x-val is deprecated.  Use image-msg:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <ping>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader image-msg:y-val is deprecated.  Use image-msg:y instead.")
  (y m))

(cl:ensure-generic-function 'r-val :lambda-list '(m))
(cl:defmethod r-val ((m <ping>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader image-msg:r-val is deprecated.  Use image-msg:r instead.")
  (r m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ping>) ostream)
  "Serializes a message object of type '<ping>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'x)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'x)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'x)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'x)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'y)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'y)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'y)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'y)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'r)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ping>) istream)
  "Deserializes a message object of type '<ping>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'x)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'x)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'x)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'x)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'y)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'y)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'y)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'y)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'r)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ping>)))
  "Returns string type for a message object of type '<ping>"
  "image/ping")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ping)))
  "Returns string type for a message object of type 'ping"
  "image/ping")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ping>)))
  "Returns md5sum for a message object of type '<ping>"
  "43a9b7aca27bce58e13d112ff91c35ad")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ping)))
  "Returns md5sum for a message object of type 'ping"
  "43a9b7aca27bce58e13d112ff91c35ad")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ping>)))
  "Returns full string definition for message of type '<ping>"
  (cl:format cl:nil "uint32 x~%uint32 y~%uint8  r~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ping)))
  "Returns full string definition for message of type 'ping"
  (cl:format cl:nil "uint32 x~%uint32 y~%uint8  r~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ping>))
  (cl:+ 0
     4
     4
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ping>))
  "Converts a ROS message object to a list"
  (cl:list 'ping
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
    (cl:cons ':r (r msg))
))
