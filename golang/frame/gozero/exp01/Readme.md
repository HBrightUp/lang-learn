source: 
https://go-zero.dev/docs/tasks/cli/grpc-demo
start server:
go run demo.go 
test:
grpcurl -plaintext 127.0.0.1:8080 demo.Demo/Ping
