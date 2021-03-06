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

#include <znc/Chan.h>
#include <znc/Modules.h>
#include <znc/User.h>

using std::vector;


class DetachHL : public CModule {
public:
	MODCONSTRUCTOR(DetachHL) {}

	virtual bool OnLoad(const CString& sArgs, CString& sMessage) {
		PutModule("Loading DetachHL module.");

    sArgs.Split(",", hList, false, "", "", false, true);
    hList.push_back(GetUser()->GetNick());

    PutModule("Phrases to highlight: ");

    for (std::vector<CString>::iterator it = hList.begin();
        it != hList.end(); ++it) {

      PutModule(*it);

    }

		return true;
	}

	virtual ~DetachHL() {
		PutModule("Module unloaded successfully.");
	}

	virtual bool OnBoot() {
		// This is called when the app starts up (only modules that are loaded in the config will get this event)
		return true;
	}

	virtual EModRet OnChanMsg(CNick& Nick, CChan& Channel, CString& sMessage) {

    if (Channel.IsDetached()) {

      for (std::vector<CString>::iterator it = hList.begin();
          it != hList.end(); ++it) {

        if ((sMessage.AsLower()).find((*it).AsLower()) != CString::npos) {
          PutModule("<" + Channel.GetName() + "/" + Nick.GetNick() + "> "
          + sMessage);
        }

        return CONTINUE;

      }

    }

		return CONTINUE;
	}

private:
    vector<CString> hList;
};

template<> void TModInfo<DetachHL>(CModInfo& Info) {
	Info.SetWikiPage("DetachHL");
	Info.SetHasArgs(true);
}

NETWORKMODULEDEFS(DetachHL, "Used to monitor highlights in detached channels")
