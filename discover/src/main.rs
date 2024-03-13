use std::{
    collections::HashSet,
    net::{IpAddr, Ipv4Addr, SocketAddr},
    str,
    sync::Arc,
};

use clap::Parser;
use tokio::{
    io::{AsyncReadExt, AsyncWriteExt},
    net::TcpListener,
    sync::Mutex,
};

#[derive(Parser)]
#[command(version, about, long_about = None)]
struct Args {
    #[arg(default_value_t = 0)]
    port: u16,
}

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    let args = Args::parse();
    let addr = SocketAddr::new(IpAddr::V4(Ipv4Addr::new(127, 0, 0, 1)), args.port);
    let listener = TcpListener::bind(addr).await?;

    println!("Discover server started at {}", listener.local_addr()?);

    let rooms = Arc::new(Mutex::new(HashSet::<SocketAddr>::new()));

    loop {
        let (mut socket, _) = listener.accept().await?;
        let rooms = rooms.clone();

        tokio::spawn(async move {
            println!("Connected {}", socket.peer_addr().unwrap());
            let mut buf = vec![0; 128];

            loop {
                let n = match socket.read(&mut buf).await {
                    Ok(n) if n == 0 => return,
                    Ok(n) => n,
                    Err(_) => return,
                };

                let request = str::from_utf8(&buf[..n])
                    .unwrap_or_default()
                    .trim_matches(char::from(0));

                let command: Vec<_> = request.split_whitespace().collect();

                match command.get(0) {
                    Some(&"connect") => {
                        if let [_, addr_str, port_str] = command[..] {
                            if let (Ok(addr), Ok(port)) =
                                (addr_str.parse::<u32>(), port_str.parse())
                            {
                                let mut rooms = rooms.lock().await;
                                rooms.insert(SocketAddr::new(
                                    IpAddr::from(Ipv4Addr::from(addr)),
                                    port,
                                ));
                            }
                        }
                    }
                    Some(&"disconnect") => {
                        if let [_, addr_str, port_str] = command[..] {
                            if let (Ok(addr), Ok(port)) =
                                (addr_str.parse::<u32>(), port_str.parse())
                            {
                                let mut rooms = rooms.lock().await;
                                rooms.remove(&SocketAddr::new(
                                    IpAddr::from(Ipv4Addr::from(addr)),
                                    port,
                                ));
                            }
                        }
                    }
                    Some(&"discover") => {
                        let rooms = rooms.lock().await;
                        let rooms_string = rooms
                            .iter()
                            .map(ToString::to_string)
                            .collect::<Vec<_>>()
                            .join(",");
                        if let Err(_) = socket.write_all(rooms_string.as_bytes()).await {
                            println!("Failed to write data to socket");
                            return;
                        }
                    }
                    _ => {}
                }
            }
        });
    }
}
