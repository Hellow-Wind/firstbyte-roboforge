#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "wpr_simulation2__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__wpr_simulation2__msg__Object() -> *const std::ffi::c_void;
}

#[link(name = "wpr_simulation2__rosidl_generator_c")]
extern "C" {
    fn wpr_simulation2__msg__Object__init(msg: *mut Object) -> bool;
    fn wpr_simulation2__msg__Object__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<Object>, size: usize) -> bool;
    fn wpr_simulation2__msg__Object__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<Object>);
    fn wpr_simulation2__msg__Object__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<Object>, out_seq: *mut rosidl_runtime_rs::Sequence<Object>) -> bool;
}

// Corresponds to wpr_simulation2__msg__Object
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Object {

    // This member is not documented.
    #[allow(missing_docs)]
    pub name: rosidl_runtime_rs::Sequence<rosidl_runtime_rs::String>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub x: rosidl_runtime_rs::Sequence<f64>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub y: rosidl_runtime_rs::Sequence<f64>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub z: rosidl_runtime_rs::Sequence<f64>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub probability: rosidl_runtime_rs::Sequence<f64>,

}



impl Default for Object {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !wpr_simulation2__msg__Object__init(&mut msg as *mut _) {
        panic!("Call to wpr_simulation2__msg__Object__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for Object {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wpr_simulation2__msg__Object__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wpr_simulation2__msg__Object__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wpr_simulation2__msg__Object__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for Object {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for Object where Self: Sized {
  const TYPE_NAME: &'static str = "wpr_simulation2/msg/Object";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__wpr_simulation2__msg__Object() }
  }
}


