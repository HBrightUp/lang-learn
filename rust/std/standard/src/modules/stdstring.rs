use std::{
    mem::{
        self,
    },
};



pub fn stdstring() {

    string_from_raw_parts();
    
    {
        //  Vec -> String;  
        let sparkle_heart = vec![240, 159, 146, 150];
        let sparkle_heart = String::from_utf8(sparkle_heart).unwrap();
        assert_eq!("💖", sparkle_heart);

        // string -> bytes
        let bytes = sparkle_heart.into_bytes();
        assert_eq!(bytes, [240, 159, 146, 150]);
    }

    // String -> Vec
    {
        let mut s = String::from("hello");

        unsafe {
            let vec = s.as_mut_vec();
            assert_eq!(&[104, 101, 108, 108, 111][..], &vec[..]);

            vec.reverse();
        }
        assert_eq!(s, "olleh");
    }

    {
        //  str -> String; String -> str;
        let mut s = String::from("abc");
        let mut drain = s.drain(..);
        assert_eq!("abc", drain.as_str());
        let _ = drain.next().unwrap();
        assert_eq!("bc", drain.as_str());
    }

    {   // String -> ref bytes
        let s = String::from("hello");  
        assert_eq!(&[104, 101,108, 108, 111], s.as_bytes());
    }

    // String -> Box<str>
    {
        let s = String::from("hello");
        let _b = s.into_boxed_str();     
    }
    //retain: Keep characters for which the condition is true
    {
        let mut s = String::from("f_o_ob_ar");
        s.retain(|c| c != '_');
        assert_eq!(s, "foobar");
    }

   
}


fn  string_from_raw_parts() {

    let mut v = vec![65, 122, 66, 67];
    let s = unsafe {
        String::from_raw_parts(v.as_mut_ptr(), v.len(), v.capacity())
    };

    //WARNING: 'v' will release memory twice, remember to call "mem::forget(v)" to avoid this situation.
    //free(): double free detected in tcache 2 , Aborted (core dumped)
    mem::forget(v);

    assert_eq!(s, "AzBC");
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_string() {
        string_from_raw_parts();
    }
}