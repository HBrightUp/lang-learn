use std::{
    fmt::{
        self,
    },
    str::{
        FromStr,
    },
};

use super::person::Person;


pub struct Docment {
    pub info: String,
    pub content: Vec<u8>,
}

impl Default for Docment {
    fn default() -> Self {
        Self {
            info: "default".to_string(),
            content: vec![],
        }
    }
}

//实现 &mut
//?Sized: 表示 T类型的大小未知;
//为 struct Docment 实现 AsMut trait，实现以后将实现 Docment -> Vec<u8> 的自动转换；
impl<T> AsMut<T> for Docment 
where 
    T: ?Sized,
    Vec<u8>: AsMut<T>
{
    fn as_mut(&mut self) -> &mut T  {
        self.content.as_mut()
    }
}

//实现 ToString trait.
// impl ToString for Docment {
//     fn to_string(&self) -> String {
//         format!("{}", self.info)
//     }
// }

// fmt::Display 和 ToString 只能实现一个，否则 Docment.to_string() 会有冲突，编译器不知道调用哪个 trait
impl fmt::Display for Docment {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "info: {}, content: {}", self.info, self.content.as_ascii().is_some())
    }
}

// 实现 From trait, 可以从 String -> Docment
impl From<String> for Docment {
    fn from(info: String) -> Self {
        Self {
            info,
            content: vec![],
        }
    }
}

// 实现 From trait, 可以从 Vec<u8> -> Docment
impl From<Vec<u8>> for Docment {
    fn from(content: Vec<u8>) -> Self {
        Self {
            info: String::from("default"),
            content,
        }
    }
}

// 实现 From trait, 可以从 Person -> Docment
impl From<&Person> for Docment {
    fn from(p: &Person) -> Self {
        Self {
            info: p.name.clone(),
            content: vec![],
        }
    }
}

#[derive(Debug, PartialEq, Eq)]
pub struct ParsePointError;

//实现 FromStr trait
impl FromStr for Docment {
    type Err = ParsePointError;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Ok(
            Self {
                info: s.to_string(),
                content: vec![],
            }
        )
    }
}


