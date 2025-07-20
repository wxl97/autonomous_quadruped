
(cl:in-package :asdf)

(defsystem "state_indicator_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "state_indicator" :depends-on ("_package_state_indicator"))
    (:file "_package_state_indicator" :depends-on ("_package"))
  ))