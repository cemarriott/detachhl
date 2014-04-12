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

USERMODULEDEFS(DetachHL, "Used to monitor highlights in detached channels")
