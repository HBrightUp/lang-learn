

pub fn stdstr() {

    //from_boxed_utf8_unchecked:  utf8 -> str
    {
        let smile_utf8 = Box::new([226, 152, 186]);
        let smile = unsafe { std::str::from_boxed_utf8_unchecked(smile_utf8)};
        assert_eq!("☺", &*smile);
    }

    //from_utf8:
    {
        let sparkle_heart = vec![240, 159, 146, 150];
        let sparkle_heart = str::from_utf8(&sparkle_heart).unwrap();
        assert_eq!("💖", sparkle_heart);
    }
}
