#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to wpr_simulation2__msg__Object

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Object {

    // This member is not documented.
    #[allow(missing_docs)]
    pub name: Vec<std::string::String>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub x: Vec<f64>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub y: Vec<f64>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub z: Vec<f64>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub probability: Vec<f64>,

}



impl Default for Object {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::Object::default())
  }
}

impl rosidl_runtime_rs::Message for Object {
  type RmwMsg = super::msg::rmw::Object;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        name: msg.name
          .into_iter()
          .map(|elem| elem.as_str().into())
          .collect(),
        x: msg.x.into(),
        y: msg.y.into(),
        z: msg.z.into(),
        probability: msg.probability.into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        name: msg.name
          .iter()
          .map(|elem| elem.as_str().into())
          .collect(),
        x: msg.x.as_slice().into(),
        y: msg.y.as_slice().into(),
        z: msg.z.as_slice().into(),
        probability: msg.probability.as_slice().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      name: msg.name
          .into_iter()
          .map(|elem| elem.to_string())
          .collect(),
      x: msg.x
          .into_iter()
          .collect(),
      y: msg.y
          .into_iter()
          .collect(),
      z: msg.z
          .into_iter()
          .collect(),
      probability: msg.probability
          .into_iter()
          .collect(),
    }
  }
}


