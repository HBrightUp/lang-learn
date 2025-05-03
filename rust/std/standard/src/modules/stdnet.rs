use std::{
    net::{
        TcpListener,Ipv4Addr,TcpStream,UdpSocket,Shutdown,SocketAddrV4
    },
    thread::{
        self,
    },
    io::{
        self,Write,Read,
    },
};

use crate::include::base::BUFFER_SIZE;


//遗留问题：Tcp server 如何检测到 client 关闭了？
pub fn stdnet() {

    //Ipv4Addr
    {
        let localhost = Ipv4Addr::new(127, 0, 0, 1);
        assert_eq!("127.0.0.1".parse(), Ok(localhost));
        assert_eq!(localhost.is_loopback(), true);
        assert!("0000000.0.0.0".parse::<Ipv4Addr>().is_err());
        assert_eq!(Ipv4Addr::parse_ascii(b"127.0.0.1"), Ok(localhost));

        let addr = Ipv4Addr::LOCALHOST;
        assert_eq!(localhost, addr);
        assert_eq!(localhost.is_private(), false);
    }

    //SocketAddrV4
    {
        let mut socket = SocketAddrV4::new(Ipv4Addr::new(127,0,0,1), 8080);
        assert_eq!("127.0.0.1:8080".parse(), Ok(socket));
        assert_eq!(socket.ip(), &Ipv4Addr::new(127, 0, 0, 1));
        assert_eq!(socket.port(), 8080);

        socket.set_ip(Ipv4Addr::new(192, 168, 0, 1));
        assert_eq!(socket.ip(), &Ipv4Addr::new(192, 168, 0, 1));

        socket.set_port(9000);
        assert_eq!(socket.port(), 9000);
    }

    //Tcp 
    {
        // TCP Server
        let listener = TcpListener::bind("127.0.0.1:9000").unwrap();
        listener.set_nonblocking(true).expect("Cannot set non-blocking");

        let listener_handle = thread::spawn( move ||{
            for stream in listener.incoming() {
                match stream {
                    Ok(mut s) => {
                        handle_client(&mut s);

                        //Remark: just for demo
                        break;
                    }
                    Err(ref e) if e.kind() == io::ErrorKind::WouldBlock => {
                        continue;
                    }
                    Err(e) => panic!("encountered io err: {e}"),
                }
            }
        });

        // TCP client
        {
            let mut client_stream = TcpStream::connect("127.0.0.1:9000").unwrap();
            client_stream.write(b"Hello, server!").unwrap();

            let mut buf = [0; BUFFER_SIZE];
            let len = client_stream.read(&mut buf).unwrap();
            let buf_string = String::from_utf8_lossy(&buf[..len]);

            println!("Tcp client recv, len: {}, data: {}", len, buf_string);

            //close
            client_stream.shutdown(Shutdown::Both).expect("shutdown call failed!");
        }

        listener_handle.join().unwrap();
    }

    //UDP 
    {   
        //UDP Server
        let us = UdpSocket::bind("127.0.0.1:8888").expect("udp server bind failed.");
        
        let udp_server_handle = thread::spawn( move ||{
            let mut buf = [0; BUFFER_SIZE];
            let (len, peer_addr) = us.recv_from(&mut buf).unwrap();
            println!("Udp server recv, len: {}, peer_addr:{} data: {:?}", len, peer_addr, &buf[..len]);

            // reverse buf
            let buf = &mut buf[..len];
            buf.reverse();
            us.send_to(buf, &peer_addr).unwrap();

        });
        
        
        //UDP Client
        {
            let us = UdpSocket::bind("127.0.0.1:8889").expect("udp client bind failed.");
            us.connect("127.0.0.1:8888").expect("connected udp server failed.");

            let _ = us.send(&[1, 2, 3, 4, 5, 6]);
            let mut buf = [0; BUFFER_SIZE];
            let len = us.recv(&mut buf).unwrap(); 
            println!("Udp client recv, len: {}, data: {:?}", len, &buf[..len]);
        
        }

        udp_server_handle.join().unwrap();
    } 
}

// deal the connection from client.
fn handle_client(stream: &mut TcpStream) {
    let mut buf = [0; BUFFER_SIZE];
    loop {
        let len = stream.read(&mut buf).unwrap();

        //client closed
        if len == 0 {
            break;
        }

        let peer_addr = stream.peer_addr().unwrap().to_string();
        let buf_string = String::from_utf8_lossy(&buf[..len]);
        println!("Tcp server recv, peer_addr: {}, len: {}, data: {}", peer_addr, len, buf_string);

        stream.write(b"Hello, client!").unwrap();
    }

}