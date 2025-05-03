

pub fn enter() {

    let uri = ["https://www.google.com/".to_string(), "https://docs.rs/solana-gossip/latest/solana_gossip/".to_string()];
    parse_uri(&uri);

    let cur_dir = std::env::current_dir().unwrap().display().to_string();
    println!("current directory: {}", cur_dir);
    parse_uri(&[cur_dir, "/home/hml/.config/solana/id2.json".to_string()]);
}


fn parse_uri(uri: &[String]) {

    for uri in uri {
        let parsed = uriparse::URIReference::try_from(uri.as_str());
        println!("<{}>: {:#?}", uri, parsed);

        if let Ok(parsed) = parsed {
            let reconstructed = format!("{}", parsed);
            if reconstructed != *uri {
                println!("Warning: URI doesn't round-trip -- serializes into:");
                println!("<{}>", reconstructed);
            }
        }
    }
}

/*  parse_uri
<https://www.google.com/>: Ok(
    URIReference {
        authority: Some(
            Authority {
                host: RegisteredName(
                    RegisteredName {
                        normalized: true,
                        registered_name: "www.google.com",
                    },
                ),
                password: None,
                port: None,
                username: None,
            },
        ),
        fragment: None,
        path: Path {
            absolute: true,
            double_dot_segment_count: 0,
            leading_double_dot_segment_count: 0,
            segments: [
                Segment {
                    normalized: true,
                    segment: "",
                },
            ],
            single_dot_segment_count: 0,
            unnormalized_count: 0,
        },
        query: None,
        scheme: Some(
            HTTPS,
        ),
    },
)

</home/hml/myself/github/lang-learn/rust/libraries>: Ok(
    URIReference {
        authority: None,
        fragment: None,
        path: Path {
            absolute: true,
            double_dot_segment_count: 0,
            leading_double_dot_segment_count: 0,
            segments: [
                Segment {
                    normalized: true,
                    segment: "home",
                },
                Segment {
                    normalized: true,
                    segment: "hml",
                },
                Segment {
                    normalized: true,
                    segment: "myself",
                },
                Segment {
                    normalized: true,
                    segment: "github",
                },
                Segment {
                    normalized: true,
                    segment: "lang-learn",
                },
                Segment {
                    normalized: true,
                    segment: "rust",
                },
                Segment {
                    normalized: true,
                    segment: "libraries",
                },
            ],
            single_dot_segment_count: 0,
            unnormalized_count: 0,
        },
        query: None,
        scheme: None,
    },

*/