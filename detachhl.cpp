/*
 * Copyright (C) 2004-2014 ZNC, see the NOTICE file for details.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <znc/Client.h>
#include <znc/Chan.h>
#include <znc/Modules.h>

using std::vector;


class DetachHL : public CModule {
public:
	MODCONSTRUCTOR(DetachHL) {}

	virtual bool OnLoad(const CString& sArgs, CString& sMessage) {
		PutModule("I'm being loaded with the arguments: [" + sArgs + "]");
		return true;
	}

	virtual ~CSampleMod() {
		PutModule("I'm being unloaded!");
	}

	virtual bool OnBoot() {
		// This is called when the app starts up (only modules that are loaded in the config will get this event)
		return true;
	}

	virtual EModRet OnInvite(const CNick& Nick, const CString& sChan) {
		if (sChan.Equals("#test")) {
			PutModule(Nick.GetNick() + " invited us to " + sChan + ", ignoring invites to " + sChan);
			return HALT;
		}

		PutModule(Nick.GetNick() + " invited us to " + sChan);
		return CONTINUE;
	}

	virtual void OnNick(const CNick& OldNick, const CString& sNewNick, const vector<CChan*>& vChans) {
		PutModule("* " + OldNick.GetNick() + " is now known as " + sNewNick);
	}

	virtual EModRet OnUserCTCPReply(CString& sTarget, CString& sMessage) {
		PutModule("[" + sTarget + "] userctcpreply [" + sMessage + "]");
		sMessage = "\037" + sMessage + "\037";

		return CONTINUE;
	}

	virtual EModRet OnCTCPReply(CNick& Nick, CString& sMessage) {
		PutModule("[" + Nick.GetNick() + "] ctcpreply [" + sMessage + "]");

		return CONTINUE;
	}

	virtual EModRet OnUserCTCP(CString& sTarget, CString& sMessage) {
		PutModule("[" + sTarget + "] userctcp [" + sMessage + "]");

		return CONTINUE;
	}

	virtual EModRet OnPrivCTCP(CNick& Nick, CString& sMessage) {
		PutModule("[" + Nick.GetNick() + "] privctcp [" + sMessage + "]");
		sMessage = "\002" + sMessage + "\002";

		return CONTINUE;
	}

	virtual EModRet OnChanCTCP(CNick& Nick, CChan& Channel, CString& sMessage) {
		PutModule("[" + Nick.GetNick() + "] chanctcp [" + sMessage + "] to [" + Channel.GetName() + "]");
		sMessage = "\00311,5 " + sMessage + " \003";

		return CONTINUE;
	}

	virtual EModRet OnUserNotice(CString& sTarget, CString& sMessage) {
		PutModule("[" + sTarget + "] usernotice [" + sMessage + "]");
		sMessage = "\037" + sMessage + "\037";

		return CONTINUE;
	}

	virtual EModRet OnPrivNotice(CNick& Nick, CString& sMessage) {
		PutModule("[" + Nick.GetNick() + "] privnotice [" + sMessage + "]");
		sMessage = "\002" + sMessage + "\002";

		return CONTINUE;
	}

	virtual EModRet OnChanNotice(CNick& Nick, CChan& Channel, CString& sMessage) {
		PutModule("[" + Nick.GetNick() + "] channotice [" + sMessage + "] to [" + Channel.GetName() + "]");
		sMessage = "\00311,5 " + sMessage + " \003";

		return CONTINUE;
	}

	virtual EModRet OnTopic(CNick& Nick, CChan& Channel, CString& sTopic) {
		PutModule("* " + Nick.GetNick() + " changes topic on " + Channel.GetName() + " to '" + sTopic + "'");

		return CONTINUE;
	}

	virtual EModRet OnUserTopic(CString& sTarget, CString& sTopic) {
		PutModule("* " + m_pClient->GetNick() + " changed topic on " + sTarget + " to '" + sTopic + "'");

		return CONTINUE;
	}

	virtual EModRet OnUserMsg(CString& sTarget, CString& sMessage) {
		PutModule("[" + sTarget + "] usermsg [" + sMessage + "]");
		sMessage = "Sample: \0034" + sMessage + "\003";

		return CONTINUE;
	}

	virtual EModRet OnPrivMsg(CNick& Nick, CString& sMessage) {
		PutModule("[" + Nick.GetNick() + "] privmsg [" + sMessage + "]");
		sMessage = "\002" + sMessage + "\002";

		return CONTINUE;
	}

	virtual EModRet OnChanMsg(CNick& Nick, CChan& Channel, CString& sMessage) {
		if (sMessage == "!ping") {
			PutIRC("PRIVMSG " + Channel.GetName() + " :PONG?");
		}

		sMessage = "x " + sMessage + " x";

		PutModule(sMessage);

		return CONTINUE;
	}
};

template<> void TModInfo<CSampleMod>(CModInfo& Info) {
	Info.SetWikiPage("sample");
	Info.SetHasArgs(true);
	Info.SetArgsHelpText("Description of module arguments goes here.");
}

USERMODULEDEFS(CSampleMod, "To be used as a sample for writing modules")

