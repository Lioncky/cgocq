#pragma once

// retcode
// https://docs.go-cqhttp.org/api
typedef unsigned CQ_RET; 
enum CQ_RET_ : unsigned {
	CQRET_OK, // status = 'ok'
	CQRET_Async, // 'async' api 调用已经提交异步处理
	CQRET_OtherFailed, // 操作失败, 具体原因可以看响应的 msg 和 wording 字段
	CQRET_MsgNotFound = 100, // 'get_msg' 消息不存在
	CQRET_Other = 888888
};

// https://docs.go-cqhttp.org/api
typedef unsigned CQ_ERROR_HTTP; 
enum CQ_ERROR_HTTP_ : unsigned {
	CQERR_HTTP200 = 200, // 操作成功
	CQERR_HTTP401 = 401, // access token 不符合
	CQERR_HTTP403 = 403, // access token 未提供
	CQERR_HTTP404 = 404, // API 不存在
	CQERR_HTTP406 = 406, // Content-Type 不支持 (非 application/json 或 application/x-www-form-urlencoded
	CQERR_HTTPSUCCESS = CQERR_HTTP200 // 除上述情况外所有情况 (具体 API 调用是否成功, 需要看 API 的 响应数据
};

typedef unsigned CQ_Define;
enum CQ_: unsigned {

};