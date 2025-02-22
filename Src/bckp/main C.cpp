#include <stdio.h>
#include <uv.h>

void on_read(uv_fs_t* req) {
    uv_fs_req_cleanup(req);
    delete req;
}

void on_open(uv_fs_t* req) {
    if (req->result >= 0) {
        uv_fs_t* read_req = new uv_fs_t;
        uv_buf_t iov = uv_buf_init((char*)req->data, MAX_PATH);
        uv_fs_read(uv_default_loop(), read_req, (uv_file)req->result, &iov, 1, -1, on_read);
    }
    else {
        fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
    }
}

int main() {
    uv_fs_t* open_req = new uv_fs_t;
    char* buffer = new char[MAX_PATH];
    ZeroMemory(buffer, MAX_PATH);
    open_req->data = buffer;
    uv_fs_open(uv_default_loop(), open_req, "./test.txt", O_RDONLY, 0, on_open);

    printf("Opening file...\n");
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    printf("%s\n", buffer);

    uv_fs_req_cleanup(open_req);
    delete open_req;
    delete[] buffer;

    uv_loop_close(uv_default_loop());
    return 0;
}

//void on_write(uv_fs_t* req) {
//    printf("AA %s\n", buffer);
//}
//
//void on_read(uv_fs_t* req) {
//    if (req->result < 0) {
//        fprintf(stderr, "Read error: %s\n", uv_strerror(req->result));
//    }
//    else if (req->result == 0) {
//        uv_fs_t close_req;
//        // synchronous
//        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
//    }
//    else if (req->result > 0) {
//        iov.len = req->result;
//        uv_fs_write(uv_default_loop(), &write_req, write_req.result, &iov, 1, -1, on_write);
//    }
//}
//
//void on_open(uv_fs_t* req) {
//    if (req->result >= 0) {
//        iov = uv_buf_init(buffer, sizeof(buffer));
//        uv_fs_read(uv_default_loop(), &read_req, req->result, &iov, 1, -1, on_read);
//    }
//    else {
//        fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
//    }
//}
//
//int main() {
//    uv_fs_open(uv_default_loop(), &open_req, "./test.txt", O_RDONLY, 0, on_open);
//
//    printf("Opening file...\n");
//    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
//
//    uv_fs_req_cleanup(&open_req);
//    uv_fs_req_cleanup(&read_req);
//    uv_fs_req_cleanup(&write_req);
//
//    uv_loop_close(uv_default_loop());
//    return 0;
//}