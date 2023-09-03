#pragma once

// retcode
// https://docs.go-cqhttp.org/api
typedef unsigned CQ_RET; 
enum CQ_RET_ : unsigned {
	CQRET_OK, // status = 'ok'
	CQRET_Async, // 'async' api �����Ѿ��ύ�첽����
	CQRET_OtherFailed, // ����ʧ��, ����ԭ����Կ���Ӧ�� msg �� wording �ֶ�
	CQRET_MsgNotFound = 100, // 'get_msg' ��Ϣ������
	CQRET_Other = 888888
};

// https://docs.go-cqhttp.org/api
typedef unsigned CQ_ERROR_HTTP; 
enum CQ_ERROR_HTTP_ : unsigned {
	CQERR_HTTP200 = 200, // �����ɹ�
	CQERR_HTTP401 = 401, // access token ������
	CQERR_HTTP403 = 403, // access token δ�ṩ
	CQERR_HTTP404 = 404, // API ������
	CQERR_HTTP406 = 406, // Content-Type ��֧�� (�� application/json �� application/x-www-form-urlencoded
	CQERR_HTTPSUCCESS = CQERR_HTTP200 // ������������������ (���� API �����Ƿ�ɹ�, ��Ҫ�� API �� ��Ӧ����
};

typedef unsigned CQ_Define;
enum CQ_: unsigned {

};