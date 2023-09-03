#include <map>
#include <WinWs.h>
#include "CQdefine.h"

class CGo : public WSocket {
public: 
	CGo() :m_postcode(0), m_wscilentport(0), m_hostnumber(0) {}
	long long gethost() { return this->m_hostnumber; }

	bool StartService(long long host, int port) {
		this->m_hostnumber = host;
		this->m_wscilentport = port;
		return this->Listen(port);
	}
	void Reconnected() override {
		this->Upgrade(); 
	}
	void UpgradeWs() override{
		CAS("Upgrade successful~-> Host [WS://127.0.0.1:%d]", m_wscilentport);
	}
	void RecvWs(PCSTR sMsg, INT szMsg) override {

		if (*sMsg != '{') {
			CAF("Unk_%d->%s", szMsg, sMsg);
			return;
		}

		// 配置转发Wnd 待更新
		// ...

		Json _;
		WDS(FMTM "WS_CQ_Recv(%u) -> \n%s", _FMTM, szMsg, sMsg);
		if (!_.parse(sMsg, szMsg)) {
			CAF(einfunc "Json Parse failed...%d->%s", szMsg, sMsg);
			dbk(einfunc);
			return;
		}

		if (_.has("echo")) {
			int iResult = _["retcode"];

			WAS("CQ::Echo->[%d]%s", iResult, sMsg);
			if (_["echo"] == "get_msg") { // 撤回消息的回调
				if (iResult == CQRET_OK) { // CQRET_MsgNotFound
					// do some thing
				}
				return;
			}

			if (iResult == CQRET_OK) {
				WDS(FMTM "[%s]=>成功 Success!", _FMTM, *_["echo"]);
			}
			else if (iResult == CQRET_Async) {
				WDS(FMTM "[%s]=>已推送到异步任务 Had post to async task lists!", _FMTM, *_["echo"]);
			}
			else {
				if (_.has("wording")) { // wording 调用API失败
					CAF(FMTM "[%s 调用失败]%s|%s|%s", _FMTM, *_["echo"], *_["msg"], *_["wording"], *_["retcode"]);
					WDA(sMsg); dbk(einfunc);
					return;
				}
				CAW(FMTM "[%s]=>%s!", _FMTM, *_["echo"], *_["message"]);
			}
			return;
		}

		if (!_.has("post_type")) {
			CAW(einfunc "[%s]字段缺少post_type !", sMsg);
			return;
		}

		// 消息事件
		if (_["post_type"] == "message") {
			Json& sender = _["sender"];
			bool group_message = _["message"] == "group";

			return;
		}

		// 通知事件
		if (_["post_type"] == "notice") {
			CAS("NOTICE=>%s", sMsg);
			auto& notice_type = _["notice_type"]; // 撤回回调 

			if (notice_type == "group_recall" || notice_type == "friend_recall") {
				// 直接调用get_msg获取消息内容
				CQ.SendWes(ESTR(R"({
					"echo": "get_msg",
					"action": "get_msg",
					"params": {
						"message_id": %Id
					})", (long long)_["message_id"]));
				return;
			}
		}

		// 心跳事件
		if (_["post_type"] == "meta_event") {
			DAS("CQ has received meta_event !!!\n");
			return;
		}
	}

	size_t Post(PCSTR action, ESTR param) {
		size_t postcode = this->QCode();
		this->SendWes(ESTR(R"({"echo": "%d#%s","action" : "%s",%s})", postcode, action, action, *param));
		return postcode;
	}

private:

	// websocket客户端端口
	int m_wscilentport;

	// hostnumber主人QQ号
	long long m_hostnumber; 

	// postcode 请求代码
	size_t m_postcode; Locker lk_postcode;

	// 取请求代码
	size_t QCode() {
		AutoLocker _(lk_postcode);
		return m_postcode++;
	}
} CQ;

size_t CQ_SFM(IINT friends, PCSTR msg) {
	return CQ.Post("send_private_msg", ESTR(R"("params": {
    "user_id": %Id,
    "message": "%s"
})",friends, msg));
}
size_t CQ_SFR(PCSTR sFlag, BOOL bAgree) {
	return CQ.Post("set_friend_add_request", ESTR(R"("params": {
    "flag": "%s",
    "approve": %s
})", sFlag, bAgree _BR));
}

size_t CQ_SGM(IINT group, PCSTR msg) {
	return CQ.Post("send_group_msg", ESTR(R"("params": {
"params": {
    "group_id": %Id,
    "message": "%s"
})", group, msg));
}
size_t CQ_SGB(IINT group, IINT friends, UINT uMinute) {
	return CQ.Post("set_group_ban", ESTR(R"("params": {
    "group_id": %Id,
    "user_id": %Id,
    "duration": "%u"
})", group, friends, uMinute * 60));
}
size_t CQ_SGK(IINT group, IINT friends, BOOL bBlock) {
	return CQ.Post("set_group_kick", ESTR(R"("params": {
    "group_id": %Id,
    "user_id": %Id,
    "reject_add_request": "%s"
})", group, friends, bBlock _BR));
}
size_t CQ_SGA(IINT group, IINT friends, BOOL bEnable) {
	return CQ.Post("set_group_admin", ESTR(R"("params": {
    "group_id": %Id,
    "user_id": %Id,
    "enable": %s
})", group, friends, bEnable _BR));
}
size_t CQ_SGC(IINT group, IINT friends, PCSTR sCard) {
	return CQ.Post("set_group_card", ESTR(R"("params": {
    "group_id": %Id,
    "user_id": %Id,
    "card": "%s"
})", group, friends, sCard));
}
size_t CQ_SGR(PCSTR sFlag, PCSTR sSubType, BOOL bAgree, PCSTR sReason) {
	return CQ.Post("set_group_add_request", ESTR( R"("params": {
    "flag": "%s",
    "sub_type": "%s",
    "approve": %s,
    "reason": "%s"
})", sFlag, sSubType, bAgree _BR, sReason)); 
}
size_t CQ_SGH(IINT group, IINT friends, PCSTR sHat) {
	return CQ.Post("set_group_special_title", ESTR(R"("params": {
    "group_id": %Id,
    "user_id": %Id,
    "special_title": "%s"
})", group, friends, sHat));
}
size_t CQ_SGL(IINT group) {
	return CQ.Post("set_group_leave", ESTR( R"("params": {
    "group_id": %Id
})", group));
}

size_t CQ_Send(IINT nQG, PCSTR fmt, ...) {
	va_list va; va_start(va, fmt);
	return CQ_SGM(nQG, ESTR(va, fmt));
}

BOOL Initalization(PCSTR _goPath, int wsPort, iint iihost) {

	static auto CQ_Port = wsPort;
	static ESTR goLaunch = _goPath;
	static ESTR goDir = WD::GetPathDir(goLaunch);

	if (!WD::IsFile(goLaunch)) {
		WD::MBErr("CQ路径无效!\n\n%s", goLaunch.data());
		WD::ExitProc();
		return FALSE;
	} goLaunch.append(" -faststart");

	CQ.StartService(iihost,wsPort);
	WD::BeginThread([](intr) {
		while (1) {
			if (CQ.IsAlive()) {
				::Sleep(5000);
				continue;
			}
			CQ.StopReconnect();

			PROCESS_INFORMATION pi;
			HANDLE& hProc = pi.hProcess;
			STARTUPINFOW StartupInfo = { sizeof(StartupInfo) };
			StartupInfo.hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);

			if (::CreateProcessW(0, goLaunch, 0, 0, FALSE, CREATE_SUSPENDED, 0, goDir, &StartupInfo, &pi)) {

				::ResumeThread(pi.hThread);
				::CloseHandle(pi.hProcess);
				::CloseHandle(pi.hThread); 

				// After CQ.StopReconnect must wait at least 3s
				::Sleep(8000); // Wait for GoCQ Start as well
				::DisableConsoleInsertPause();
				::SetConsoleTitleW(L"CGoCQ_Console");
				if (CQ.Listen(CQ_Port)) {
					CAS("[SUCCESS] CQ Has been connected!");
				}
			}
		};
	});
	return TRUE;
}

int main(int arg, char* argv[]) {

	WD::UConsole();

	// GlobalConfigInit 
	WD::InitIni(L"./cgo.ini");
	{
		char sPath[0x200];
		int port; long long host;
		WD::GetIni(L"GC", L"CQ_PortWs", port, 8080);

		WD::GetIniString(L"GC", L"CQ_Host", sPath, 0x200); host = atoll(sPath);

		WD::GetIniString(L"GC", L"CQ_Path", sPath, 0x200);

		Initalization(sPath, port, host);
	}

	static CHAR tmp[MEM_GRANULARITY];
	HANDLE hin = ::GetStdHandle(STD_INPUT_HANDLE);
	UINT64 tick = ::GetTickCount64(); PSTR p = tmp;
	do {

		::ReadFile(hin, p, 1, 0, 0);
		if (*p++ == '\r') {
			::ReadFile(hin, p, 1, 0, 0); // "\n"
			_uint(--p) = '\0'; p = tmp;
			switch (_uint(p)) {
				case "cls"_uu:
					WD::ClearConsole();
					break;
				
				case 't': case 'tt': case 'ttt': case "chm"_uu: { // cq host message
					CAW("CQ_HOST_MSG");

					CQ_SFM(CQ.gethost(), ESTR("%d", WD::Tick(8)));
					break;
				}
				
				case "kcq"_uu: {
					CAW("KILL_REBOT_CQ");

					WD::ShellKill("godev.exe");
					break;
				}

				// help is the end
				case 'h': case 'H':
				case "-h\0"_uu: case "-H\0"_uu: case "help"_uu: {

					CAW(FMTM "HELP...", _FMTM);
					CAW("t/chm...CQ_HOST_MSG");
					CAW("kcq...KILL_REBOT_CQ");

					goto lab_running;
				} // There is no break

			default:
				lab_running:
					CAW("Now Running...%s",
						*WD::Time2Info(::GetTickCount64() - tick)
					);
					break;
			}
		}
	} while (true);
}

#if 0

#define CQ_POST "127.0.0.1:5700"

ESTR CQ_OpenUrl(PCSTR sUrl) {
	return WS::OpenUrl(sUrl);
}

// CQ_Api for get data
BOOL CQ_Api(Json& js, PCSTR nUrl) {
	WDS(einfunc "%s", nUrl);
	ESTR esr = WS::OpenUrl(ESTR("%s%s", CQ_POST, nUrl));
	return esr ? (js.parse(esr, esr.size())) : FALSE;
}
ESTR CQ_QNick(PCSTR nQQ) {
	Json js;
	if (CQ_Api(js, ESTR("get_stranger_info?user_id=%s", nQQ))) {
		if (js["data"].has("nickname")) return *js["data"]["nickname"];
	}
	return "";
}
ESTR CQ_QGroupName(PCSTR nQG) {
	Json js; CQ_Api(js, ESTR("get_group_info?group_id=%s", nQG));
	return (js.has("group_name")) ? (PSTR)js["group_name"] : (PSTR)"";
}

#endif
