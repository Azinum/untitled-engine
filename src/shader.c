// shader.c

char diffuse2_vert[] = {
  0x2f, 0x2f, 0x20, 0x64, 0x69, 0x66, 0x66, 0x75, 0x73, 0x65, 0x32, 0x2e, 0x76, 0x65, 
  0x72, 0x74, 0x0a, 0x0a, 0x23, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x31, 
  0x35, 0x30, 0x0a, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x70, 0x6f, 
  0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 
  0x32, 0x20, 0x75, 0x76, 0x3b, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 
  0x6e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x3b, 0x0a, 0x0a, 0x6f, 0x75, 0x74, 0x20, 0x76, 
  0x65, 0x63, 0x32, 0x20, 0x75, 0x76, 0x5f, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x3b, 0x0a, 
  0x6f, 0x75, 0x74, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x73, 0x75, 0x72, 0x66, 0x61, 
  0x63, 0x65, 0x5f, 0x6e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x3b, 0x0a, 0x6f, 0x75, 0x74, 
  0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x66, 0x72, 0x61, 0x67, 0x5f, 0x70, 0x6f, 0x73, 
  0x3b, 0x0a, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, 0x61, 0x74, 
  0x34, 0x20, 0x70, 0x72, 0x6f, 0x6a, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 
  0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, 0x61, 0x74, 0x34, 0x20, 0x76, 
  0x69, 0x65, 0x77, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, 
  0x61, 0x74, 0x34, 0x20, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x3b, 0x0a, 0x0a, 0x76, 0x6f, 
  0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x28, 0x29, 0x20, 0x7b, 0x0a, 0x09, 0x75, 
  0x76, 0x5f, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63, 0x32, 
  0x28, 0x75, 0x76, 0x2e, 0x78, 0x2c, 0x20, 0x31, 0x20, 0x2d, 0x20, 0x75, 0x76, 0x2e, 
  0x79, 0x29, 0x3b, 0x0a, 0x09, 0x73, 0x75, 0x72, 0x66, 0x61, 0x63, 0x65, 0x5f, 0x6e, 
  0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x20, 0x3d, 0x20, 0x6e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 
  0x3b, 0x0a, 0x09, 0x66, 0x72, 0x61, 0x67, 0x5f, 0x70, 0x6f, 0x73, 0x20, 0x3d, 0x20, 
  0x76, 0x65, 0x63, 0x33, 0x28, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x20, 0x2a, 0x20, 0x76, 
  0x65, 0x63, 0x34, 0x28, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x2c, 0x20, 
  0x31, 0x29, 0x29, 0x3b, 0x0a, 0x09, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 
  0x69, 0x6f, 0x6e, 0x20, 0x3d, 0x20, 0x70, 0x72, 0x6f, 0x6a, 0x65, 0x63, 0x74, 0x69, 
  0x6f, 0x6e, 0x20, 0x2a, 0x20, 0x76, 0x69, 0x65, 0x77, 0x20, 0x2a, 0x20, 0x6d, 0x6f, 
  0x64, 0x65, 0x6c, 0x20, 0x2a, 0x20, 0x76, 0x65, 0x63, 0x34, 0x28, 0x70, 0x6f, 0x73, 
  0x69, 0x74, 0x69, 0x6f, 0x6e, 0x2c, 0x20, 0x31, 0x29, 0x3b, 0x0a, 0x7d, 0x0a, 0x00, };

char diffuse_vert[] = {
  0x2f, 0x2f, 0x20, 0x64, 0x69, 0x66, 0x66, 0x75, 0x73, 0x65, 0x2e, 0x76, 0x65, 0x72, 
  0x74, 0x0a, 0x0a, 0x23, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x31, 0x35, 
  0x30, 0x0a, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x70, 0x6f, 0x73, 
  0x69, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x32, 
  0x20, 0x75, 0x76, 0x3b, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x6e, 
  0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x3b, 0x0a, 0x0a, 0x6f, 0x75, 0x74, 0x20, 0x76, 0x65, 
  0x63, 0x32, 0x20, 0x75, 0x76, 0x5f, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x3b, 0x0a, 0x6f, 
  0x75, 0x74, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x73, 0x75, 0x72, 0x66, 0x61, 0x63, 
  0x65, 0x5f, 0x6e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x3b, 0x0a, 0x6f, 0x75, 0x74, 0x20, 
  0x76, 0x65, 0x63, 0x33, 0x20, 0x66, 0x72, 0x61, 0x67, 0x5f, 0x70, 0x6f, 0x73, 0x3b, 
  0x0a, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, 0x61, 0x74, 0x34, 
  0x20, 0x70, 0x72, 0x6f, 0x6a, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x75, 
  0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, 0x61, 0x74, 0x34, 0x20, 0x76, 0x69, 
  0x65, 0x77, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, 0x61, 
  0x74, 0x34, 0x20, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x3b, 0x0a, 0x0a, 0x76, 0x6f, 0x69, 
  0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x28, 0x29, 0x20, 0x7b, 0x0a, 0x09, 0x75, 0x76, 
  0x5f, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28, 
  0x75, 0x76, 0x2e, 0x78, 0x2c, 0x20, 0x31, 0x20, 0x2d, 0x20, 0x75, 0x76, 0x2e, 0x79, 
  0x29, 0x3b, 0x0a, 0x09, 0x73, 0x75, 0x72, 0x66, 0x61, 0x63, 0x65, 0x5f, 0x6e, 0x6f, 
  0x72, 0x6d, 0x61, 0x6c, 0x20, 0x3d, 0x20, 0x6e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x3b, 
  0x0a, 0x09, 0x66, 0x72, 0x61, 0x67, 0x5f, 0x70, 0x6f, 0x73, 0x20, 0x3d, 0x20, 0x76, 
  0x65, 0x63, 0x33, 0x28, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x20, 0x2a, 0x20, 0x76, 0x65, 
  0x63, 0x34, 0x28, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x2c, 0x20, 0x31, 
  0x29, 0x29, 0x3b, 0x0a, 0x09, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 
  0x6f, 0x6e, 0x20, 0x3d, 0x20, 0x70, 0x72, 0x6f, 0x6a, 0x65, 0x63, 0x74, 0x69, 0x6f, 
  0x6e, 0x20, 0x2a, 0x20, 0x76, 0x69, 0x65, 0x77, 0x20, 0x2a, 0x20, 0x6d, 0x6f, 0x64, 
  0x65, 0x6c, 0x20, 0x2a, 0x20, 0x76, 0x65, 0x63, 0x34, 0x28, 0x70, 0x6f, 0x73, 0x69, 
  0x74, 0x69, 0x6f, 0x6e, 0x2c, 0x20, 0x31, 0x29, 0x3b, 0x0a, 0x7d, 0x0a, 0x00, };

char sprite_vert[] = {
  0x2f, 0x2f, 0x20, 0x73, 0x70, 0x72, 0x69, 0x74, 0x65, 0x2e, 0x76, 0x65, 0x72, 0x74, 
  0x0a, 0x0a, 0x23, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x31, 0x35, 0x30, 
  0x0a, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x76, 0x65, 0x72, 0x74, 
  0x65, 0x78, 0x3b, 0x0a, 0x6f, 0x75, 0x74, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x75, 
  0x76, 0x3b, 0x0a, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, 0x61, 
  0x74, 0x34, 0x20, 0x70, 0x72, 0x6f, 0x6a, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 
  0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, 0x61, 0x74, 0x34, 0x20, 
  0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x3b, 0x0a, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 
  0x61, 0x69, 0x6e, 0x28, 0x29, 0x20, 0x7b, 0x0a, 0x09, 0x75, 0x76, 0x20, 0x3d, 0x20, 
  0x76, 0x65, 0x72, 0x74, 0x65, 0x78, 0x2e, 0x7a, 0x77, 0x3b, 0x0a, 0x09, 0x67, 0x6c, 
  0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x3d, 0x20, 0x70, 0x72, 
  0x6f, 0x6a, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x2a, 0x20, 0x6d, 0x6f, 0x64, 
  0x65, 0x6c, 0x20, 0x2a, 0x20, 0x76, 0x65, 0x63, 0x34, 0x28, 0x76, 0x65, 0x72, 0x74, 
  0x65, 0x78, 0x2e, 0x78, 0x79, 0x2c, 0x20, 0x30, 0x2c, 0x20, 0x31, 0x29, 0x3b, 0x0a, 
  0x7d, 0x0a, 0x00, };

char diffuse2_frag[] = {
  0x2f, 0x2f, 0x20, 0x64, 0x69, 0x66, 0x66, 0x75, 0x73, 0x65, 0x32, 0x2e, 0x66, 0x72, 
  0x61, 0x67, 0x0a, 0x0a, 0x23, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x31, 
  0x35, 0x30, 0x0a, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x75, 0x76, 
  0x5f, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x3b, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 
  0x33, 0x20, 0x73, 0x75, 0x72, 0x66, 0x61, 0x63, 0x65, 0x5f, 0x6e, 0x6f, 0x72, 0x6d, 
  0x61, 0x6c, 0x3b, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x66, 0x72, 
  0x61, 0x67, 0x5f, 0x70, 0x6f, 0x73, 0x3b, 0x0a, 0x0a, 0x6f, 0x75, 0x74, 0x20, 0x76, 
  0x65, 0x63, 0x34, 0x20, 0x6f, 0x75, 0x74, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3b, 
  0x0a, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x73, 0x61, 0x6d, 0x70, 
  0x6c, 0x65, 0x72, 0x32, 0x44, 0x20, 0x74, 0x65, 0x78, 0x3b, 0x0a, 0x0a, 0x76, 0x65, 
  0x63, 0x33, 0x20, 0x6c, 0x69, 0x67, 0x68, 0x74, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63, 
  0x33, 0x28, 0x30, 0x2c, 0x20, 0x30, 0x2c, 0x20, 0x30, 0x29, 0x3b, 0x0a, 0x76, 0x65, 
  0x63, 0x33, 0x20, 0x6c, 0x69, 0x67, 0x68, 0x74, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 
  0x20, 0x3d, 0x20, 0x76, 0x65, 0x63, 0x33, 0x28, 0x31, 0x2c, 0x20, 0x31, 0x2c, 0x20, 
  0x31, 0x29, 0x3b, 0x0a, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 
  0x28, 0x29, 0x20, 0x7b, 0x0a, 0x09, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, 0x61, 0x6d, 
  0x62, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x73, 0x74, 0x72, 0x65, 0x6e, 0x67, 0x74, 0x68, 
  0x20, 0x3d, 0x20, 0x30, 0x2e, 0x31, 0x3b, 0x0a, 0x09, 0x76, 0x65, 0x63, 0x33, 0x20, 
  0x61, 0x6d, 0x62, 0x69, 0x65, 0x6e, 0x74, 0x20, 0x3d, 0x20, 0x61, 0x6d, 0x62, 0x69, 
  0x65, 0x6e, 0x74, 0x5f, 0x73, 0x74, 0x72, 0x65, 0x6e, 0x67, 0x74, 0x68, 0x20, 0x2a, 
  0x20, 0x6c, 0x69, 0x67, 0x68, 0x74, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3b, 0x0a, 
  0x0a, 0x09, 0x76, 0x65, 0x63, 0x33, 0x20, 0x6e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x20, 
  0x3d, 0x20, 0x6e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x69, 0x7a, 0x65, 0x28, 0x73, 0x75, 
  0x72, 0x66, 0x61, 0x63, 0x65, 0x5f, 0x6e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x29, 0x3b, 
  0x0a, 0x09, 0x76, 0x65, 0x63, 0x33, 0x20, 0x6c, 0x69, 0x67, 0x68, 0x74, 0x5f, 0x64, 
  0x69, 0x72, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x3d, 0x20, 0x6e, 0x6f, 0x72, 
  0x6d, 0x61, 0x6c, 0x69, 0x7a, 0x65, 0x28, 0x6c, 0x69, 0x67, 0x68, 0x74, 0x20, 0x2d, 
  0x20, 0x66, 0x72, 0x61, 0x67, 0x5f, 0x70, 0x6f, 0x73, 0x29, 0x3b, 0x0a, 0x09, 0x66, 
  0x6c, 0x6f, 0x61, 0x74, 0x20, 0x64, 0x65, 0x6c, 0x74, 0x61, 0x20, 0x3d, 0x20, 0x6d, 
  0x61, 0x78, 0x28, 0x64, 0x6f, 0x74, 0x28, 0x6e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x2c, 
  0x20, 0x6c, 0x69, 0x67, 0x68, 0x74, 0x5f, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x69, 
  0x6f, 0x6e, 0x29, 0x2c, 0x20, 0x30, 0x2e, 0x30, 0x29, 0x3b, 0x0a, 0x09, 0x76, 0x65, 
  0x63, 0x33, 0x20, 0x64, 0x69, 0x66, 0x66, 0x75, 0x73, 0x65, 0x20, 0x3d, 0x20, 0x64, 
  0x65, 0x6c, 0x74, 0x61, 0x20, 0x2a, 0x20, 0x6c, 0x69, 0x67, 0x68, 0x74, 0x5f, 0x63, 
  0x6f, 0x6c, 0x6f, 0x72, 0x3b, 0x0a, 0x0a, 0x09, 0x6f, 0x75, 0x74, 0x5f, 0x63, 0x6f, 
  0x6c, 0x6f, 0x72, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63, 0x34, 0x28, 0x28, 0x61, 0x6d, 
  0x62, 0x69, 0x65, 0x6e, 0x74, 0x20, 0x2b, 0x20, 0x64, 0x69, 0x66, 0x66, 0x75, 0x73, 
  0x65, 0x29, 0x2c, 0x20, 0x31, 0x2e, 0x30, 0x29, 0x20, 0x2a, 0x20, 0x74, 0x65, 0x78, 
  0x74, 0x75, 0x72, 0x65, 0x32, 0x44, 0x28, 0x74, 0x65, 0x78, 0x2c, 0x20, 0x75, 0x76, 
  0x5f, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x29, 0x3b, 0x0a, 0x7d, 0x0a, 0x00, };

char diffuse_frag[] = {
  0x2f, 0x2f, 0x20, 0x64, 0x69, 0x66, 0x66, 0x75, 0x73, 0x65, 0x2e, 0x66, 0x72, 0x61, 
  0x67, 0x0a, 0x0a, 0x23, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x31, 0x35, 
  0x30, 0x0a, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x75, 0x76, 0x5f, 
  0x63, 0x6f, 0x6f, 0x72, 0x64, 0x3b, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x33, 
  0x20, 0x73, 0x75, 0x72, 0x66, 0x61, 0x63, 0x65, 0x5f, 0x6e, 0x6f, 0x72, 0x6d, 0x61, 
  0x6c, 0x3b, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x66, 0x72, 0x61, 
  0x67, 0x5f, 0x70, 0x6f, 0x73, 0x3b, 0x0a, 0x0a, 0x6f, 0x75, 0x74, 0x20, 0x76, 0x65, 
  0x63, 0x34, 0x20, 0x6f, 0x75, 0x74, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3b, 0x0a, 
  0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x73, 0x61, 0x6d, 0x70, 0x6c, 
  0x65, 0x72, 0x32, 0x44, 0x20, 0x74, 0x65, 0x78, 0x3b, 0x0a, 0x0a, 0x76, 0x6f, 0x69, 
  0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x28, 0x29, 0x20, 0x7b, 0x0a, 0x09, 0x6f, 0x75, 
  0x74, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x3d, 0x20, 0x74, 0x65, 0x78, 0x74, 
  0x75, 0x72, 0x65, 0x28, 0x74, 0x65, 0x78, 0x2c, 0x20, 0x75, 0x76, 0x5f, 0x63, 0x6f, 
  0x6f, 0x72, 0x64, 0x29, 0x3b, 0x0a, 0x7d, 0x0a, 0x00, };

char sprite_frag[] = {
  0x2f, 0x2f, 0x20, 0x73, 0x70, 0x72, 0x69, 0x74, 0x65, 0x2e, 0x66, 0x72, 0x61, 0x67, 
  0x0a, 0x0a, 0x23, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x31, 0x35, 0x30, 
  0x0a, 0x0a, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x75, 0x76, 0x3b, 0x0a, 
  0x6f, 0x75, 0x74, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 
  0x3b, 0x0a, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x73, 0x61, 0x6d, 
  0x70, 0x6c, 0x65, 0x72, 0x32, 0x44, 0x20, 0x74, 0x65, 0x78, 0x3b, 0x0a, 0x75, 0x6e, 
  0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x6f, 0x66, 0x66, 
  0x73, 0x65, 0x74, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x76, 
  0x65, 0x63, 0x32, 0x20, 0x72, 0x61, 0x6e, 0x67, 0x65, 0x3b, 0x0a, 0x0a, 0x76, 0x6f, 
  0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x28, 0x29, 0x20, 0x7b, 0x0a, 0x09, 0x63, 
  0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63, 0x34, 0x28, 0x74, 0x65, 
  0x78, 0x74, 0x75, 0x72, 0x65, 0x28, 0x74, 0x65, 0x78, 0x2c, 0x20, 0x28, 0x75, 0x76, 
  0x20, 0x2a, 0x20, 0x72, 0x61, 0x6e, 0x67, 0x65, 0x29, 0x20, 0x2b, 0x20, 0x6f, 0x66, 
  0x66, 0x73, 0x65, 0x74, 0x29, 0x2e, 0x72, 0x67, 0x62, 0x2c, 0x20, 0x31, 0x29, 0x3b, 
  0x0a, 0x09, 0x69, 0x66, 0x20, 0x28, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x2e, 0x72, 0x20, 
  0x3d, 0x3d, 0x20, 0x31, 0x20, 0x26, 0x26, 0x20, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x2e, 
  0x67, 0x20, 0x3d, 0x3d, 0x20, 0x30, 0x20, 0x26, 0x26, 0x20, 0x63, 0x6f, 0x6c, 0x6f, 
  0x72, 0x2e, 0x62, 0x20, 0x3d, 0x3d, 0x20, 0x31, 0x29, 0x20, 0x7b, 0x0a, 0x09, 0x09, 
  0x64, 0x69, 0x73, 0x63, 0x61, 0x72, 0x64, 0x3b, 0x0a, 0x09, 0x7d, 0x0a, 0x7d, 0x0a, 
  0x00, };

char diffuse2_attribs[] = {
  0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x70, 0x6f, 0x73, 0x69, 
  0x74, 0x69, 0x6f, 0x6e, 0x20, 0x30, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 
  0x74, 0x65, 0x20, 0x75, 0x76, 0x20, 0x31, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 
  0x75, 0x74, 0x65, 0x20, 0x6e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x20, 0x32, 0x0a, 0x00, };

char diffuse_attribs[] = {
  0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x70, 0x6f, 0x73, 0x69, 
  0x74, 0x69, 0x6f, 0x6e, 0x20, 0x30, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 
  0x74, 0x65, 0x20, 0x75, 0x76, 0x20, 0x31, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 
  0x75, 0x74, 0x65, 0x20, 0x6e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x20, 0x32, 0x0a, 0x00, };

