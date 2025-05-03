use std::io::Cursor;
use byteorder::{BigEndian, ReadBytesExt, LittleEndian, WriteBytesExt};

pub fn enter() {
    bytes_big_endian();
    bytes_little_endian();
}


fn bytes_big_endian() {
    let mut rdr = Cursor::new(vec![2, 5, 3, 0]);
    assert_eq!(517, rdr.read_u16::<BigEndian>().unwrap()); // [2, 5]
    assert_eq!(768, rdr.read_u16::<BigEndian>().unwrap());  // [3,0]
}

fn bytes_little_endian() {
    let mut wtr = vec![];
    wtr.write_u16::<LittleEndian>(517).unwrap(); // [5, 2]
    wtr.write_u16::<LittleEndian>(768).unwrap(); // [0, 3]
    assert_eq!(wtr, vec![5, 2, 0, 3]);
}

