**Bài tập lập trình khóa học "Phát triển giao thức mạng 4G-LTE" của Viettel High Technology Industries Corporation**
======================
**SCORE SCHOOL SYSTEM**

### 1.Thông tin sinh viên:
----------------------

- Họ và tên: Nguyễn Đình Nam
- Mã SV: B15DCVT262
- Lớp: D16CQVT06-B
- Trường: Học viện Công nghệ Bưu chính Viễn Thông


### 2. Mô tả yêu cầu:
----------------------

#### 2.1. Giới thiệu:

    Mini project: TCP user client và TCP user server có 3 process:
- TCP user server:
	đóng vai trò là TCP user server,
	chứa thông tin của các client: client_id, client_password, client_score...
	trả về kết quả truy nhập và điểm số khi có yêu cầu truy nhập từ client
    	
- TCP user client 1 (hoặc client_1)
	đóng vai trò là TCP user client,
	gửi yêu cầu truy nhập và yêu cầu điểm số của client_1 đến server

- TCP user client 2 (hoặc client_2)
	đóng vai trò là TCP user client,
	gửi yêu cầu truy nhập và yêu cầu điểm số của client_2 đến server

#### 2.2. Flow hoạt động:
 
    TCP user client 1 (C1), TCP user server (S)
	1. C1 gửi cho S bản tin LOG_IN_REQUEST // S xử lý trên thread_1
	2. S gửi cho C1 bản tin LOG_IN_SUCCESS

    	3. C1 gửi cho S bản tin SCORE_REQUEST
	4. S gửi cho C1 bản tin SCORE_SUCCESS

	5. C1 gửi cho S bản tin LOG_OUT_REQUEST
	6. S gửi cho C1 bản tin LOG_OUT_SUCCESS

	7. C1 gửi cho S bản tin SCORE_REQUEST
	8. S gửi cho C1 bản tin SCORE_FAILURE

    TCP user client 2 (C2), TCP user server (S)
	1. C2 gửi cho S bản tin LOG_IN_REQUEST // S xử lý trên thread_2
	2. S gửi cho C2 bản tin LOG_IN_SUCCESS

#### 2.3 Lưu ý
- Ngôn ngữ C, sử dụng bộ thư viện TCP trên hệ điều hành Linux
- LOG_IN_REQUEST có ít nhất (Client_id và client_password)
- TCP user server sử dụng thread_1 cho client_1 và thread_2 cho client_2
