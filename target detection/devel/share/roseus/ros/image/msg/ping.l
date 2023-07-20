;; Auto-generated. Do not edit!


(when (boundp 'image::ping)
  (if (not (find-package "IMAGE"))
    (make-package "IMAGE"))
  (shadow 'ping (find-package "IMAGE")))
(unless (find-package "IMAGE::PING")
  (make-package "IMAGE::PING"))

(in-package "ROS")
;;//! \htmlinclude ping.msg.html


(defclass image::ping
  :super ros::object
  :slots (_x _y _r ))

(defmethod image::ping
  (:init
   (&key
    ((:x __x) 0)
    ((:y __y) 0)
    ((:r __r) 0)
    )
   (send-super :init)
   (setq _x (round __x))
   (setq _y (round __y))
   (setq _r (round __r))
   self)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:r
   (&optional __r)
   (if __r (setq _r __r)) _r)
  (:serialization-length
   ()
   (+
    ;; uint32 _x
    4
    ;; uint32 _y
    4
    ;; uint8 _r
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32 _x
       (write-long _x s)
     ;; uint32 _y
       (write-long _y s)
     ;; uint8 _r
       (write-byte _r s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32 _x
     (setq _x (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _y
     (setq _y (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint8 _r
     (setq _r (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get image::ping :md5sum-) "43a9b7aca27bce58e13d112ff91c35ad")
(setf (get image::ping :datatype-) "image/ping")
(setf (get image::ping :definition-)
      "uint32 x
uint32 y
uint8  r

")



(provide :image/ping "43a9b7aca27bce58e13d112ff91c35ad")


