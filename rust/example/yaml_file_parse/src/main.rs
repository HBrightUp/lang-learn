use std::collections::HashMap;
use std::fmt::{Debug, Display};
use std::fs::File;
use std::io::Read;
use serde::{Deserialize, Serialize};
use anyhow::Result;

#[derive(Debug, Serialize, Deserialize)]
struct DockerYaml {
    version: String,
    services: HashMap<String, ServiceCfg>,

    // If the Network is not configured when loading the file, do not generate it when saving the file.
    #[serde(skip_serializing_if = "HashMap::is_empty", default)]
    networks: HashMap<String, NetworkCfg>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct ServiceCfg {
    image: String,
    #[serde(skip_serializing_if = "String::is_empty", default)]
    restart: String,
    #[serde(skip_serializing_if = "Option::is_none", default)]
    privileged: Option<bool>,
    #[serde(skip_serializing_if = "Vec::is_empty", default)]
    ports: Vec<String>,
    #[serde(skip_serializing_if = "Vec::is_empty", default)]
    volumes: Vec<String>,
    #[serde(skip_serializing_if = "String::is_empty", default)]
    command: String,
    #[serde(skip_serializing_if = "Vec::is_empty", default)]
    depends_on: Vec<String>,
}


#[derive(Debug, Clone, Serialize, Deserialize)]
struct NetworkCfg {
    #[serde(default)]
    driver: Option<String>,
    #[serde(default)]
    attachable: Option<bool>,
}



fn main() -> Result<()> {
    let mut file = File::open("./src/docker-compose.yaml")?; // 打开文件
    let mut contents = String::new(); // 创建一个字符串
    file.read_to_string(&mut contents)?; // 读取文件内容到字符串缓冲区
    // println!("读取到的 YAML 内容:\n{}", contents); // 打印原始的YAML字符串
    // 将 YAML 字符串反序列化为 DockerYaml 结构体
    let docker_yaml: DockerYaml = serde_yaml::from_str(&contents)?;
    // 打印反序列化后的结构体
    println!("反序列化后的结构体: {:?}", docker_yaml);
    // 读取出 services
    let services = &docker_yaml.services;
    for (serve, config) in services {
        println!("services name: {}", serve);
        println!("services config: {:?}", config);
    }
    // 读取出 networks
    let networks = &docker_yaml.networks;
    for (serve, config) in networks {
        println!("networks name: {}", serve);
        println!("networks config: {:?}", config);
    }
    // 将数据结构序列化为 YAML 字符串
    let yaml_string = serde_yaml::to_string(&docker_yaml)?;
    println!("序列化为 YAML 字符串:\n{}", yaml_string); // 打印序列化后的 YAML 字符串

    let save_path = "./src/docker-compose-deal.yaml";
    let _ = std::fs::write(save_path, yaml_string);
    Ok(())

}
