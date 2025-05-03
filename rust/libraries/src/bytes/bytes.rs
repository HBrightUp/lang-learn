


pub fn enter() {
    
}


#[cfg(test)]
mod test {

    use bytes::{
        Bytes, BytesMut,Buf, BufMut,
    };

    #[test]
    fn test_bytes_mut() {
        let mut buf = BytesMut::with_capacity(1024);
        buf.put(&b"Hello, rust!"[..]);
        buf.put_u16(1234);

        let a = buf.split();
        assert_eq!(a, b"Hello, rust!\x04\xD2"[..]);
        
        buf.put(&b"goodbye world"[..]);

        let b = buf.split();
        assert_eq!(b, b"goodbye world"[..]);

        assert_eq!(buf.capacity(), 997);

        let c = buf.split();
        assert_eq!(c, "");
    }

    #[test]
    fn test_bytes() {
        let mut mem = Bytes::from("Hello world");
        assert_eq!(mem.len(), 11);

        let a = mem.slice(0..5);

        assert_eq!(a, "Hello");

        let b = mem.split_to(6);

        assert_eq!(mem, "world");
        assert_eq!(b, "Hello ");
    }

    #[test]
    fn test_buf() {
        let mut buf = &b"hello world"[..];

        assert_eq!(b'h', buf.get_u8());
        assert_eq!(b'e', buf.get_u8());
        assert_eq!(b'l', buf.get_u8());

        let mut rest = [0; 8];
        buf.copy_to_slice(&mut rest);

        assert_eq!(&rest[..], &b"lo world"[..]);
    }
}