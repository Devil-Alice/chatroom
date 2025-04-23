GRPC_BIN_DIR=/root/.local/bin

$GRPC_BIN_DIR/protoc -I="." --grpc_out="." --plugin=protoc-gen-grpc="$GRPC_BIN_DIR/grpc_cpp_plugin" ./my_grpc.proto

$GRPC_BIN_DIR/protoc --cpp_out=. ./my_grpc.proto