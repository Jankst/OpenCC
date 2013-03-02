/*
* Open Chinese Convert
*
* Copyright 2013 BYVoid <byvoid.kcp@gmail.com>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <iostream>
#include <string>

#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/var.h>
#include <json/json.h>

#include "../src/opencc.h"

namespace {

using std::string;
using std::cerr;
using std::cout;
using std::endl;

class OpenccInstance : public pp::Instance {
 public:
  explicit OpenccInstance(PP_Instance instance) : pp::Instance(instance) {
  }

  virtual ~OpenccInstance() {
  }

  virtual void HandleMessage(const pp::Var &message) {
    if (!message.is_string()) {
      return;
    }
    string json_string = message.AsString();
    Json::Value root;
    Json::Reader reader;
    bool parsedSucc = reader.parse(json_string, root);
    if (!parsedSucc) {
      cerr << "Error parsing json: " << json_string << endl;
      return;
    }
    const Json::Value command = root["command"];
    if (!command.isString()) {
      cerr << "Invalid format: property 'command' isn't string" << endl;
      return;
    }
    string command_name = command.asString();
    if (command_name == "convert") {
      const Json::Value text = root["text"];
      if (!text.isString()) {
        cerr << "Invalid format: property 'text' isn't string" << endl;
        return;
      }
      string input = text.asString();
      const Json::Value config_value = root["config"];
      if (!config_value.isString()) {
        cerr << "Invalid format: property 'config' isn't string" << endl;
        return;
      }
      string config = config_value.asString();
      Convert(config, input);
    }
  }

 private:
  void PostMessage(const Json::Value& res) {
    Json::FastWriter writer;
    string json_string = writer.write(res);
    pp::Instance::PostMessage(pp::Var(json_string));
  }

  void Convert(string& config, string& input) {
    opencc_t od = opencc_open(config.c_str());
    if (od == (opencc_t) -1) {
      cerr << "Can not create opencc object" << endl;
      return;
    }
    char* output = opencc_convert_utf8(od, input.c_str(), -1);
    Json::Value res;
    res["type"] = "convert";
    res["text"] = output;
    free(output);
    opencc_close(od);
    PostMessage(res);
  }
};

class OpenccModule : public pp::Module {
 public:
  OpenccModule() : pp::Module() {}
  virtual ~OpenccModule() {}

  virtual pp::Instance *CreateInstance(PP_Instance instance) {
    return new OpenccInstance(instance);
  }
};

}  // namespace

namespace pp {
  Module *CreateModule() {
    return new OpenccModule();
  }
}
