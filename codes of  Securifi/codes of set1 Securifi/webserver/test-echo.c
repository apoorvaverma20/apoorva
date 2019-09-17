/*
 * libwebsockets-test-echo - libwebsockets echo test implementation
 *
 * This implements both the client and server sides.  It defaults to
 * serving, use --client <remote address> to connect as client.
 *
 * Copyright (C) 2010-2013 Andy Green <andy@warmcat.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation:
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#define ZIGBEE
#define OZW
#define PHILIPSHUE
#define NEST
#define ZWaveDefined
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>
#include <sstream>
#include<bits/stdc++.h>

#include <libAlmondHA.h>
#include <libAlmondHA_Private.h>
#include <Logger.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include "assoc.h"
#include "attribute-id.h"
#include "cluster-id.h"
#include "test-echo.h"
#include "mcrypt.h"
#include <libAlmondDBM.h>

//char str[180];
#include "data_packet.h"//new
struct login_details {
    char origin[200];
    int count;
    unsigned long long epoch;
};
extern DevType baseDevType;
//void sendFirmwareResponse(bool success, int rc, int per);




#include <sendLogs.h>
long long getMac(void);
Logger *logger;
volatile bool s_data = true;

bool fw_thread = true;
#ifndef _WIN32
#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#endif

#ifdef CMAKE_BUILD
#include "lws_config.h"
#endif

#include <mesh.h>
static MeshSlaveList ml;

struct libwebsocket *Affiliationwsi;
char t_temp[1024];
void send_data(struct libwebsocket *wsi);
void post_data(int checkValue, struct libwebsocket *wsi, char *mii, struct json_token *arr);
void post_clients_data(int checkValue, struct libwebsocket *wsi, char *mii, struct json_token *arr);
void getRuleList_as_json1(char *JSON);
void getLuaRuleEntry_as_json2(Rules &r, char *buf);
bool gettriggerandresult_New(char * Num, char *ruleid, struct json_token *tt, int TrigRes);
void getDeviceList_as_json_New(char *JSON);
void getLuaDeviceEntry_as_json_New(Device &d, char* buf);

void getIndexList_as_json_New(char* JSON);
void getLuaIndexEntry_as_json_New(Index &d, char* buf);
//void encryptpassword(char * loginpswd, char * uptime);
//void decryptpassword(char * loginpswd, char * uptime);
//void b64_decode(char *b64src, char *clrdst);
//void b64_encode(char *clrstr, char *b64dst);
//int getIfMac(const char *ifname, char *if_addr);
void encryptpassword(char * loginpswd, char * uptime);
void decryptpassword(char * loginpswd, char * uptime);
void b64_decode(char *b64src, char *clrdst);
void b64_encode(char *clrstr, char *b64dst);
int getIfMac(const char *ifname, char *if_addr);

bool th_1 = true;
HUE_DATA_PACKET Hue_Sending_packet;
NEST_DATA_PACKET Nest_Sending_Packet;
char ARRAY[50][50];
bool checkValidIndexTypeForRule(IndexType IT);
int MODE_1;
char EMAIL[256], ALMONDNAME[200];
char typeip[50], cmdtype[50];
RMemory mem;

extern "C" {
#include "../lib/private-libwebsockets.h"
#include "routerdata.c"
}
#include "language.h"
#include "assoc.cpp"
/*
void
libwebsocket_close_and_free_session(struct libwebsocket_context *context,
                               struct libwebsocket *wsi, enum lws_close_status);
char *
lws_hdr_simple_ptr(struct libwebsocket *wsi, enum lws_token_indexes h);
*/

//static volatile int force_exit = 0;
static int versa;

#define MAX_ECHO_PAYLOAD 256000
#define LOCAL_RESOURCE_PATH INSTALL_DATADIR"/libwebsockets-test-server"


static ClientsList cl;
static ScenesList sl;
Almond al;
enum RET_CODE {
    SUCC = 0, FAIL = 1, RETRY = 2
};


long long getMac(void) {
    
        long long AlmondMAC;
        FILE *fp;
        char path[180];

        fp = popen("hexdump -C /dev/mtd3 -s 4 -n 6 -e '16/1 \"%.2x\" \"\n\"' | sed -n 2p ", "r");

        if (fp == NULL) {
                printf("Failed to run command\n" );
                return 0;
        } else {
                if(fgets(path, sizeof(path)-1, fp) != NULL) {
                      //  printf("%s", path);
                        sscanf(path, "%llx", &AlmondMAC );
                      //  printf("%llu\n",AlmondMAC);
                } else {
                        printf("output empty no mac\n");
                }
                pclose(fp);
                return AlmondMAC;
        }
        return 0;
}
int email_isvalid(char *address) {
    int count = 0;
    const char *c, *domain;
    static char *rfc822_specials = (char *) "()<>@,;:\\\"[]";
    for (c = address; *c; c++) {
        if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) ==
                '\"')) {
            while (*++c) {
                if (*c == '\"') break;
                if (*c == '\\' && (*++c == ' ')) continue;
                if (*c <= ' ' || *c >= 127) return 0;
            }
            if (!*c++) return 0;
            if (*c == '@') break;
            if (*c != '.') return 0;
            continue;
        }
        if (*c == '@') break;
        if (*c <= ' ' || *c >= 127) return 0;
        if (strchr(rfc822_specials, *c)) return 0;
    }
    if (c == address || *(c - 1) == '.') return 0;

    if (!*(domain = ++c)) return 0;
    do {
        if (*c == '.') {
            if (c == domain || *(c - 1) == '.') return 0;
            count++;
        }
        if (*c <= ' ' || *c >= 127) return 0;
        if (strchr(rfc822_specials, *c)) return 0;
    } while (*++c);

    return (count >= 1);

}

bool is_number(char *num) {
    if (strlen(num) == 10) {
        unsigned int i;
        for (i = 0; i < strlen(num); i++) {
            if (!(num[i] >= '0' && num[i] <= '9'))
                return false;
}

        return true;
    } else {
        return false;
    }
}
int getJsonValue(const char *str, char* value, struct json_token *arr, int len) {
    struct json_token *tok = find_json_token(arr, str);
    if (tok) {
        snprintf(value, len, "%.*s", tok->len, tok->ptr);
        free(tok);
        tok = NULL;
        return 1; //success
    } else {
        sprintf(t_temp, "\nERROR:NO Data %s field found\n\n", str);
        LOGINFO(t_temp);
        return 0; //failure
    }
}


int lua_setDevicePropertyAsync(int dev, const char *val, const char *val1) {
    char *name, *location;
    int r = SUCC;
    try{
        Device d(dev);
        char val_buf[64], val_buf1[64];
        strncpy(val_buf, val, 64);
        val_buf[63] = '\0';
        strncpy(val_buf1, val1, 64);
        val_buf1[63] = '\0';
        name = d.getDeviceName();
        if (strlen(val)) {
            if (strcmp(name, val_buf) != 0) {
                d.setName(val_buf);
            }
        }
        if (strlen(val1)) {
            location = d.getDeviceLocation();
            if (strcmp(location, val_buf1) != 0) {
                d.setLocation(val_buf1);
            }
        }
        r = SUCC;
    } catch (int idx) {
        r = FAIL;
    }
    return (r);
}




void getLuaClientsEntry_as_json(Client &c, char* buf) {
    char Name[200] = "";
    strcpy(Name, c.getName());
    strcpy(Name, specialchar(Name));
     char schedule[100];
    c.getScheduleData(schedule);
    snprintf(buf, 1024, "\"%d\":{\"ID\":\"%d\",\"Name\":\"%s\",\"Connection\":\"%s\",\"MAC\":\"%s\",\
    \"Type\":\"%s\",\"LastKnownIP\":\"%s\",\"Active\":\"%s\",\"UseAsPresence\":\"%s\",\"LastActiveEpoch\":\"%llu\",\
    \"Wait\":\"%d\",\"Block\":\"%d\",\"RSSI\":\"%d\",\"Schedule\":\"%s\",\"RX\":\"%llu\",\"TX\":\"%llu\",\"ForceInactive\":\"%d\",\"Manufacturer\":\"%s\",\"CanBlock\":\"%s\",\"Category\":\"%s\",\"SMEnable\":\"%s\",\"BWEnable\":\"%s\",\"StaticIP\":\"%s\"}", c.getClientID(), c.getClientID(), Name, c.getConnectionType(), c.getMac(), ClientTypeToString(c.getType()),
            c.getIP(), (c.isActive() ? "true" : "false"), (c.isUseAsPresence() ? "true" : "false"), c.getLastActiveEpoch(), c.getWait(), c.getBlock(), c.getRssi(), schedule, c.getRX(), c.getTX(), c.getForceInactive(), c.getVendor(), (c.canBlock() ? "true" : "false"), c.getUserType(),(c.isSMEnable() ? "true" : "false"), (c.isBWEnable() ? "true" : "false"), (c.isStaticIP()? "true" : "false"));
}

void getClientList_as_json(char *JSON) {
    char buf[1024] = "", localbuf[3] = "";


 cl.populate();
    list<Client>::iterator it;
    int i;
    for (i = 0, it = cl.clientsList.begin(); it != cl.clientsList.end(); ++it, ++i) {
        getLuaClientsEntry_as_json(*it, buf);
        if (i > 0) {
            sprintf(localbuf, ",");
            strcat(JSON, localbuf);
    }
        strcat(JSON, buf);
    }
}

void getLuaScenesEntry_as_json(Scene &s, char * buf) {
    //    std::stringstream ss;
    char Name[200] = "", loaclbuf[512] = "";
    strcpy(Name, s.getSceneName());
    strcpy(Name, specialchar(Name));
    snprintf(buf, 1024, "\"%d\":{\"ID\":\"%d\",\"Name\":\"%s\",\"LastActiveEpoch\":\"%llu\",\"Active\":\"%s\",\"VoiceCompatible\":\"%s\",\"SceneEntryList\":[",
           s.getID(), s.getID(), Name, s.LastActivatedTime, (s.getState() ? "true" : "false"), (s.getVoiceCompatible() ? "true" : "false"));
    list<SceneEntry> sEntry;
    list<SceneEntry>::iterator sEntryiter;
    sEntry = s.getSceneEntries();
    int i = 0;
    if (!sEntry.empty()) {
        for (sEntryiter = sEntry.begin(); sEntryiter != sEntry.end(); ++sEntryiter, ++i) {
            if (i > 0) {

                strcat(buf, ",");
        }
            snprintf(loaclbuf, 512, "{\"ID\":\"%d\",\"Index\":\"%d\",\"Valid\":\"%s\",\"Value\":\"%s\"}",
                    (*sEntryiter).DeviceID, (*sEntryiter).Index, ((*sEntryiter).Valid ? "true" : "false"), (*sEntryiter).Value);
            //            ss << LBRACE;
            //            ss << APOS << "DeviceID" << APOS << COLON <<
            //                    APOS << (*sEntryiter).DeviceID << APOS << COMMA;
            //            ss << APOS << "Index" << APOS << COLON <<
            //                    APOS << (*sEntryiter).Index << APOS << COMMA;
            //            ss << APOS << "Valid" << APOS << COLON <<
            //                    APOS << ((*sEntryiter).Valid ? "true" : "false") << APOS << COMMA;
            //            ss << APOS << "Value" << APOS << COLON <<
            //                    APOS << (*sEntryiter).Value << APOS;
            //            ss << RBRACE;
            strcat(buf, loaclbuf);
    }
}
    strcat(buf, "]}");



}

void getSceneList_as_json(char * JSON) {
    //    std::stringstream ss;

	sl.Populate();
    char buf[4096] = "", localbuf[3] = "";
    list<Scene>::iterator it;
    int i;
    for (i = 0, it = sl.Scenes.begin(); it != sl.Scenes.end(); ++it, ++i) {
        getLuaScenesEntry_as_json(*it, buf);
        if (i > 0) {
            //            ss << COMMA;
            sprintf(localbuf, ",");
            strcat(JSON, localbuf);
    }
        strcat(JSON, buf);
}
}

//int getAlmondWirelessMode() {
//   char wmode[180];
//    mem.getSetting((char *) "OperationMode", wmode);
//    if (atoi(wmode) == 3 || atoi(wmode) == 9) {
//        return 1;
//    }
//    return 0;
//}

static int
callback_echo(struct libwebsocket_context *context,
		struct libwebsocket *wsi,
		enum libwebsocket_callback_reasons reason, void *user,
        void *in, size_t len) {
    static std::list<struct login_details> log_details;
	switch (reason) {
         case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
        {
            std::list<struct login_details>::iterator itlog;
            char *s, *ptr, pp[180] = {0}, uu[180] = {0}, uRec[180] = {0}, pRec[180] = {0};
            bool matched = false;
            FILE *fp;
            fp = popen("nvram_get 2860 Login", "r");
            if (fp != NULL) {
                fgets(uu, 180, fp);
                uu[strlen(uu) - 1] = '\0';
                pclose(fp);
            }
            fp = popen("nvram_get 2860 Password", "r");
            if (fp != NULL) {
                fgets(pp, 180, fp);
                pp[strlen(pp) - 1] = '\0';
                pclose(fp);
            }
            s = lws_hdr_simple_ptr(wsi, WSI_TOKEN_GET_URI);
            unsigned long long epoch;
            epoch = time(NULL);
            //            LOG(Logger::INFO, s);
                        LOG(Logger::INFO, cli_ip);
            //            LOG(Logger::INFO, uu);
            //            LOG(Logger::INFO, pp);
            if ((ptr = strchr(s + 1, ':')) || (ptr = strchr(s + 1, '/'))) {//Token  found then check credentials otherwise close socket
                strncpy(uRec, s + 1, ptr - s - 1);
                strcpy(pRec, ptr + 1);
                //LOG(Logger::INFO, pRec);
                //LOG(Logger::INFO, uRec);
                if (strcmp(pRec, pp) != 0 || strcmp(uRec, uu) != 0) {//Admin password mismatched
                    matched = false;
                    LOG(Logger::ERROR, "Password and username mismatched");
                } else {
                    matched = true;
                }
            } else {
                matched = false;
            }

            for (itlog = log_details.begin(); itlog != log_details.end(); itlog++) {
                if (strcmp(itlog->origin, cli_ip) == 0) {
                    break;
                }
            }
            if (itlog != log_details.end()) {

                if ((epoch - itlog->epoch) < 1800 && itlog->count >= 3) {//Again got same  entry with reached threshold we have to check count and drop connection.
                    //reject
                    LOG(Logger::ERROR, "Blocked for 30 mins");
                    goto REJECT;
                } else if (itlog->count <= 3 && !matched) {//Wrong password and increase count
                    //increase count and reject
                    itlog->count++;
                    itlog->epoch = epoch;
                    LOG(Logger::ERROR, "Blocked with increment count");
                    goto REJECT;
                } else {
                    //Dont know what else we can do here
                    LOG(Logger::ERROR, "Removed entry from list");
                    log_details.erase(itlog);
                }
            } else if (!matched) {//Add new entry for a client
                struct login_details lD;
                lD.count = 1;
                lD.epoch = epoch;
                sprintf(lD.origin, "%s", cli_ip);
                log_details.push_back(lD);
                LOG(Logger::ERROR, "Blocked with 1st attempt");
                goto REJECT;
            }

            break;
REJECT:
            libwebsocket_close_and_free_session(context, wsi, LWS_CLOSE_STATUS_POLICY_VIOLATION);
            LOG(Logger::ERROR, "Closed and freed connection after wrong attempt");
            lwsl_err("Closed connection returning -1 from callback");
            return -1;
        }
	/* when the callback is used for server operations --> */
        case LWS_CALLBACK_ESTABLISHED:
        {
            send_data(wsi);
        }
            break;

	case LWS_CALLBACK_SERVER_WRITEABLE:
        {
            LOG(Logger::INFO, "LWS_CALLBACK_SERVER_WRITEABLE");
		}
		break;

	case LWS_CALLBACK_RECEIVE:
        {
		if (len > MAX_ECHO_PAYLOAD) {
			lwsl_err("Server received packet bigger than %u, hanging up\n", MAX_ECHO_PAYLOAD);
			return 1;
		}
            struct json_token *arr = NULL;
            int checkValue;
            char receivedCmd[len + 1];
            memcpy(receivedCmd, in, len);
            receivedCmd[len + 1] = '\0';
            LOGINFO(receivedCmd);
            // Tokenize json string, fill in tokens array

            arr = parse_json2(receivedCmd, strlen(receivedCmd));
		//LOG(Logger::INFO,receivedCmd);
            if (arr == NULL) {
                unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 50 + LWS_SEND_BUFFER_POST_PADDING];
                snprintf((char*) &buf[LWS_SEND_BUFFER_PRE_PADDING], 50, "{\"CommandType\":\"Invalid Command\"}");
                libwebsocket_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], strlen((char*) &buf[LWS_SEND_BUFFER_PRE_PADDING]), LWS_WRITE_TEXT);
            } else {
                struct json_token *tok = NULL;
                char cmd[500] = "", mii[500] = "";
                    tok = find_json_token(arr, "CommandType");
                    if (tok == NULL) {
                        unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 50 + LWS_SEND_BUFFER_POST_PADDING];
                    snprintf((char*) &buf[LWS_SEND_BUFFER_PRE_PADDING], 50, "{\"CommandType\":\"Unknown\"}");
                        libwebsocket_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], strlen((char*) &buf[LWS_SEND_BUFFER_PRE_PADDING]), LWS_WRITE_TEXT);
                    } else {
                        if (getJsonValue("MobileInternalIndex", mii, arr, 500)) {
                            snprintf(cmd, 500, "%.*s", tok->len, tok->ptr);
                            checkValue = getCmdValue(cmd);
                            post_clients_data(checkValue, wsi, mii, arr);
                        } else {
                            unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 50 + LWS_SEND_BUFFER_POST_PADDING];
                        snprintf((char*) &buf[LWS_SEND_BUFFER_PRE_PADDING], 50, "{\"CommandType\":\"Invalid Json Data\"}");
                            libwebsocket_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], strlen((char*) &buf[LWS_SEND_BUFFER_PRE_PADDING]), LWS_WRITE_TEXT);
                        }
                    }
                if (tok) {
                free(tok);
                tok = NULL;
            }
            }
            if (arr) {
            free(arr);
            arr = NULL;
        }
        }
         
		break;

	/* when the callback is used for client operations --> */

        

	case LWS_CALLBACK_CLOSED:
            if (addwsi == wsi) {
                th_1 = true;
                alAssoc->closeAssocConnection();
                LOG(Logger::INFO, "LWS_CALLBACK_CLOSED 1111111111");
            }
            LOG(Logger::INFO, "LWS_CALLBACK_CLOSED");
            break;

	default:
		break;
	}

	return 0;
}



static struct libwebsocket_protocols protocols[] = {
	/* first protocol must always be HTTP handler */

	{
		"default",		/* name */
		callback_echo,		/* callback */
        0
	},
	{
		NULL, NULL, 0		/* End of list */
	}
};

void broadcastData(char * buffer_local)
{
    libwebsocket_broadcast(&protocols[0], buffer_local);
}

static int setValue(int dev, int idx, const char *val) {
    if (dev <= 0 || idx <= 0 || strlen(val) == 0) {
        return (FAIL);
    }

    try{
        //call libAlmond's setValue
        Device d(dev);
        if (idx > d.getValueCount()) {
            return FAIL;
        } else {
            char val_buf[64];
            strncpy(val_buf, val, 64);
            d.setValue(idx, val_buf);
            return SUCC;
        }
    } catch (int idx) {

        return FAIL;
    }
}

int lua_getDeviceVal(int dev, int idx, char *rval) {
    int ret;

    if (dev <= 0 || idx <= 0) {
        return (FAIL);
    }

    try{
        Device d(dev);
        if (idx > d.getValueCount()) {
            ret = FAIL;
        } else {
            char *val = d.getValue(idx);
            strcpy(rval, val);
            ret = SUCC;
        }
    } catch (int idx) {
        ret = FAIL;
    }
    return ret;
}
/*
void sighandler(int sig) {
	force_exit = 1;
}
*/

static struct option options[] = {
	{ "help",	no_argument,		NULL, 'h' },
	{ "debug",	required_argument,	NULL, 'd' },
	{ "port",	required_argument,	NULL, 'p' },
	{ "ssl-cert",	required_argument, 	NULL, 'C' },
	{ "ssl-key",	required_argument,	NULL, 'k' },
#ifndef LWS_NO_CLIENT
	{ "client",	required_argument,	NULL, 'c' },
	{ "ratems",	required_argument,	NULL, 'r' },
#endif
	{ "ssl",	no_argument,		NULL, 's' },
	{ "versa",	no_argument,		NULL, 'v' },
	{ "uri",	required_argument,	NULL, 'u' },
	{ "passphrase", required_argument,	NULL, 'P' },
	{ "interface",  required_argument,	NULL, 'i' },
#ifndef LWS_NO_DAEMONIZE
	{ "daemonize", 	no_argument,		NULL, 'D' },
#endif
	{ NULL, 0, 0, 0 }
};

bool checkForDeviceTriggerValidation(RuleTrigger trigger) {
    bool retb = false;
    char Vname[50] = "";
    int indexType;
    try{
        Device d(trigger.DeviceID);
        strcpy(Vname, d.getValueName(trigger.DeviceIndex));
        if (strcmp(Vname, "#ERR")) {
            retb = true;
        }
        if (trigger.ConditionType != eq) {
            indexType = d.getIndexType(trigger.DeviceIndex);
            LOG(Logger::INFO, "Inside trigger condition check");
            //            LOG(Logger::INFO, indexType);
            if (indexType != -1) {
                //                IndexTypeEnumToStr(atoi(indexType), indexType, 0);
                //                LOG(Logger::INFO, indexType);
                //                int iT = StrToIndexTypeEnum(indexType);
                //                sprintf(Vname, "IT=%d", iT);
                //                LOG(Logger::INFO, Vname);
                if (checkValidIndexTypeForRule((IndexType) indexType)) {
                    LOG(Logger::INFO, "Index type returned true");
                    retb = true;
                } else {
                    LOG(Logger::INFO, "Index type returned false");
                    retb = false;
                }
            } else {
                retb = false;
            }
        } else {
            LOG(Logger::INFO, "No need to check device trigger condition");
        }
    } catch (int i) {
        char tmpb[200];
        snprintf(tmpb, 200, "Device %d doesn't exists in DeviceList checking for Rule\n", trigger.DeviceID);
        LOG(Logger::INFO, tmpb);
    }
    return retb;
}

bool checkForTimeTriggerValidation(RuleTrigger trigger) {
    bool retb = false;
    if (trigger.timeTrigger.hours >= 0 && trigger.timeTrigger.hours < 24 && trigger.timeTrigger.mins < 60 && trigger.timeTrigger.mins >= 0) {
        if (trigger.timeTrigger.range >= 0 && trigger.timeTrigger.range < 1440) {
            retb = true;
        }
    }
    return retb;
}

bool checkForDeviceResultValidation(RuleResult ruleResult) {
    bool retb = false;
    char Vname[50] = "";
    try{
        Device d(ruleResult.DeviceID);
        strcpy(Vname, d.getValueName(ruleResult.DeviceIndex));
        if (strcmp(Vname, "#ERR")) {
            retb = true;
        }
    } catch (int i) {
        char tmpb[200];
        snprintf(tmpb, 200, "Device %d doesn't exists in DeviceList checking for Rule\n", ruleResult.DeviceID);
        LOG(Logger::INFO, tmpb);
    }
    return retb;


}

bool checkForSceneResultValidation(RuleResult ruleResult) {
    bool retb = false;
    Scene s(ruleResult.SceneID);
    if (s.getID() != 0) {
        retb = true;
    } else {
        //        fprintf(stderr, "ERROR:Invalid scene ID:%d\n", ruleResult.SceneID);
        LOG(Logger::INFO, "ERROR:Invalid scene ID:%d\n");
    }
    return retb;
}

bool checkForEmailValidation(RuleResult ruleResult) {
    bool retb = false;
    if (email_isvalid(ruleResult.msgValue) != 0) {
        retb = true;
    } else
        LOG(Logger::INFO, "*****ERROR:Invalid Email ID*******");
    return retb;
}

int AddDevicetoscene(int sceneid, int dev, int idx, const char *val) {
    if (sceneid <= 0 || idx <= 0 || strlen(val) == 0) {
        return (FAIL);
    }

    try{
        Scene s(sceneid);
        char val_buf[64];
        strncpy(val_buf, val, 64);
        if (dev == 0) {
            s.addOrUpdateSceneEntry(dev, idx, val_buf);
            return SUCC;
        } else {
            try{
                Device d(dev);

                s.addOrUpdateSceneEntry(dev, idx, val_buf);
                return SUCC;
            } catch (int dev) {
                return FAIL;
            }

        }

    } catch (int sceneid) {
        return FAIL;
    }

}

int getIfMac(const char *ifname, char *if_addr) {
    struct ifreq ifr;
    int skfd = 0;

    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("getIfMac: open socket error");
        sprintf(if_addr, "<no_mac>");
        return -1;
    }

    strncpy(ifr.ifr_name, ifname, IF_NAMESIZE);
    if (ioctl(skfd, SIOCGIFHWADDR, &ifr) < 0) {

        printf("getIfMac: ioctl SIOCGIFADDR error for %s", ifname);
        sprintf(if_addr, "<no_mac>");
        return -1;
    }
    //printf("Mac=%02x\n",ifr.ifr_hwaddr.sa_data[5]-0xffffff00);
    sprintf(if_addr, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
            (unsigned char) ifr.ifr_hwaddr.sa_data[0],
            (unsigned char) ifr.ifr_hwaddr.sa_data[1],
            (unsigned char) ifr.ifr_hwaddr.sa_data[2],
            (unsigned char) ifr.ifr_hwaddr.sa_data[3],
            (unsigned char) ifr.ifr_hwaddr.sa_data[4],
            (unsigned char) ifr.ifr_hwaddr.sa_data[5]);

    close(skfd);
    fflush(stderr);
    return 0;
}
int decrypt(void* buffer, int buffer_len, unsigned char* IV, unsigned char* key, int key_len) {
    MCRYPT td = mcrypt_module_open((char *) MCRYPT_RIJNDAEL_128, NULL, (char *) "cbc", NULL);
    int blocksize = mcrypt_enc_get_block_size(td);
    if (buffer_len % blocksize != 0) {

        return 1;
    }

    mcrypt_generic_init(td, key, key_len, IV);
    mdecrypt_generic(td, buffer, buffer_len);
    mcrypt_generic_deinit(td);
    mcrypt_module_close(td);
    return 0;
}

int encrypt(void* buffer, int buffer_len, char* IV, char* key, int key_len) { /* Because the plaintext could include null bytes*/
    MCRYPT td = mcrypt_module_open((char *) MCRYPT_RIJNDAEL_128, NULL, (char *) "cbc", NULL);
    int blocksize = mcrypt_enc_get_block_size(td);
    if (buffer_len % blocksize != 0) {
        return 1;
    }

    mcrypt_generic_init(td, key, key_len, IV);
    mcrypt_generic(td, buffer, buffer_len);
    mcrypt_generic_deinit(td);
    mcrypt_module_close(td);

    return 0;
}

void decryptpassword(char * loginpswd, char * uptime) {
    //    char uptime[33];
    printf("loginpswd in encrypt - %s\n", loginpswd);
    unsigned char key[] = {110, 204, 148, 237, 106, 144, 1, 61, 48, 175, 82, 13, 24, 119, 68, 47, 163, 185, 216, 110, 197, 9, 110, 231, 233, 18, 6, 173, 196, 89, 47, 146};
    unsigned char IV[17];
    char MAC[180];
    unsigned long long int AL_MAC = 0;
    AL_MAC = getMac();	
    sprintf(MAC,"%llu",AL_MAC);
    //    struct sysinfo info;
    //    sysinfo(&info);
    //    memset(uptime, 0, sizeof (uptime));
    //    sprintf(uptime, "%lu", info.uptime);
    memset(IV, '\0', sizeof (IV));
    int i;
    for (i = 0; i < 16; i++) {
        IV[i] = (uptime[i] + MAC[i]) % 94 + 33;
    }
    char encryptBuff[32]; //, decryptBuff[32];
    memset(encryptBuff, 0, sizeof (encryptBuff));
    sprintf((char *) encryptBuff, "%s", loginpswd);

    decrypt(encryptBuff, sizeof (encryptBuff), IV, key, 16);
    sprintf((char *) loginpswd, "%s", encryptBuff);
    printf("dddddncryptBuff - %s\n", encryptBuff);
}

void encryptpassword(char * loginpswd, char * uptime) {
    unsigned char key[] = {110, 204, 148, 237, 106, 144, 1, 61, 48, 175, 82, 13, 24, 119, 68, 47, 163, 185, 216, 110, 197, 9, 110, 231, 233, 18, 6, 173, 196, 89, 47, 146};
    unsigned char IV[17];
    char MAC[180];
   unsigned long long int AL_MAC = 0;
    AL_MAC = getMac();	
    sprintf(MAC,"%llu",AL_MAC);
    struct sysinfo info;
    sysinfo(&info);
    memset(uptime, 0, sizeof (uptime));
    sprintf(uptime, "%lu", info.uptime);
    memset(IV, '\0', sizeof (IV));
    int i;
    for (i = 0; i < 16; i++) {
        IV[i] = (uptime[i] + MAC[i]) % 94 + 33;
    }
    char encryptBuff[32]; //, decryptBuff[32];
    memset(encryptBuff, 0, sizeof (encryptBuff));
    sprintf((char *) encryptBuff, "%s", loginpswd);

    encrypt(encryptBuff, sizeof (encryptBuff), (char *) IV, (char*) key, 16);
    sprintf((char *) loginpswd, "%s", encryptBuff);
    printf("encryptBuff - %s\n", encryptBuff);
}
//void getvariableswithvalue_array(char *varaible, char *value, char *buf) {
//    //    std::stringstream ss;
//    //    ss << LBRACE;
//    //    ss << APOS << "Variable" << APOS << COLON << APOS << varaible << APOS << COMMA <<
//    //            APOS << "Value" << APOS << COLON << APOS << value << APOS;
//    //    ss << RBRACE;
//    snprintf(buf, 512, "{\"Variable\":\"%s\",\"Value\":\"%s\"}", varaible, value);
//}
//
//
//
//size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
//    return size*nmemb;
//}
//
//int checkConnection2(void) {
//    char stream[1000];
//    FILE *fp;
//
//    memset(stream, '\0', sizeof (stream));
//
//    system("ping -c 1 -w 1 8.8.8.8 > temp_file");
//
//    fp = fopen("temp_file", "r");
//
//    if (fp == NULL)
//        return (0);
//
//    fread(stream, 1, 1000, fp);
//    fclose(fp);
//
//    system("rm temp_file");
//
//    if (strstr(stream, ", 0% packet loss"))
//        return (1);
//    else
//        return (0);
//}
//
//int checkConnection(void) {
//    if (checkConnection2() == 1) {
//        return 1;
//    }
//    CURL *curl;
//    CURLcode res;
//    res_init();
//    curl = curl_easy_init();
//    if (curl) {
//
//        res = curl_easy_setopt(curl, CURLOPT_URL, "www.google.com");
//        if (res != CURLE_OK) {
//            curl_easy_cleanup(curl);
//            return 0;
//        }
//        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//        if (res != CURLE_OK) {
//            curl_easy_cleanup(curl);
//            return 0;
//        }
//        res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
//        if (res != CURLE_OK) {
//            curl_easy_cleanup(curl);
//            return false;
//        }
//        if ((res = curl_easy_perform(curl)) != CURLE_OK) {
//            switch (res) {
//                case CURLE_COULDNT_CONNECT:
//                case CURLE_COULDNT_RESOLVE_HOST:
//                case CURLE_COULDNT_RESOLVE_PROXY:
//                    printf("Internet does not exist\n");
//                    break;
//                default: printf("Request failed:\n%s\n", curl_easy_strerror(res));
//                    ;
//                    break;
//            }
//            curl_easy_cleanup(curl);
//            return 0;
//        }
//        /* always cleanup */
//
//        curl_easy_cleanup(curl);
//        return 1;
//    }
//    return 0;
//}

//int checkCables(void) {
//    int fd, value, ret = 0;
//    fd = open("/dev/almond_link", O_RDONLY);
//    if (fd < 0) {
//        system("mknod /dev/almond_link c 35 0");
//        fd = open("/dev/almond_link", O_RDONLY);
//    }
//
//    ioctl(fd, 100, &value);
//    if (value != 0)
//        ret = 1;
//
//    ioctl(fd, 200, &value);
//    if (value != 0)
//        ret = ret + (10);
//
//    ioctl(fd, 300, &value);
//    if (value != 0)
//        ret = ret + (100);
//
//    close(fd);
//    return ret;
//}
//
//int checkWANCable(void) {
//    int fd, value;
//    fd = open("/dev/almond_link", O_RDONLY);
//    if (fd < 0) {
//        system("mknod /dev/almond_link c 35 0");
//        fd = open("/dev/almond_link", O_RDONLY);
//    }
//
//    ioctl(fd, 100, &value);
//    close(fd);
//    
//    if (value != 0)
//        return 1;
//    else
//        return 0;
//}
//
//int checkCableAndInternet(void) {
//    char output[180];
//    mem.getSetting((char *) "OperationMode", output);
//
//    if (output[0] == '1' || output[0] == '9') {
//        if (checkWANCable()) {
//            return checkConnection();
//        } else {
//            return -1;
//    }
//    } else
//        return checkConnection();
//}
//
//typedef struct {
//    char channel[3];
//    char ssid[50];
//    char bssid[50];
//    char authmode[15];
//    char encryption[10];
//    int signal_strength;
//    char w_mode[8];
//
//} ssinfo;
//
//int wpa2try = 0;
//int wpawpa2mode = 0;
//int tkipaesmode = 0;
//int aestry = 0;
//
//
//
//
//
//void disable_rangeExtender(void) {
//    char output[180];
//    mem.getSetting("OperationMode", output); // if in repeater mode
//    if (output[0] == '3') {
//        //system("killall monitor");
//        //        mW.setLine1(lan.get("disablel1", "repeater"));
//        //        mW.setLine2(lan.get("disablel2", "repeater"));
//        //        mW.setLine3(lan.get("disablel3", "repeater"));
//        //        mW.drawTimed(MODAL_TYPE_CRITICAL);
//        char oldLanIp[20], oldDhcpStart[20], oldDhcpEnd[20], oldDhcpGateway[20], oldDhcpDNS[20];
//
//        mem.getSetting("old_lan_ipaddr", oldLanIp);
//        mem.getSetting("old_dhcpEnd", oldDhcpEnd);
//        mem.getSetting("old_dhcpStart", oldDhcpStart);
//        mem.getSetting("old_dhcpGateway", oldDhcpGateway);
//        mem.getSetting("old_dhcpPriDns", oldDhcpDNS);
//
//        if (strlen(oldLanIp) > 5) {
//            mem.addValue("lan_ipaddr", oldLanIp);
//            mem.addValue("dhcpEnd", oldDhcpEnd);
//            mem.addValue("dhcpStart", oldDhcpStart);
//            mem.addValue("dhcpGateway", oldDhcpGateway);
//            mem.addValue("dhcpPriDns", oldDhcpDNS);
//        }
//        mem.addValue("OperationMode", "1");
//        mem.saveValues();
//        system("internet.sh&");
//    }
//}
//
//int isRangeExtenderEnabled(void){
//    char output[180];
//    mem.getSetting("OperationMode", output);
//    if(output[0] != '3')
//        return -1;
//    getIfIp("br0", output);
//    if(output[0] == '<' || (!(output[0] >= '1' && output[0] <= '9')))
//        return 0;
//    else
//        return 1;
//}
//int showSteps(ssinfo current, char *apPassword, bool isReconnecting) {
//    char /*apPassword[180],*/ oldLanIp[20], oldDhcpStart[20], oldDhcpEnd[20], oldDhcpGateway[20], oldDhcpDNS[20];
//    char output[180], ip[16];
//    //    passwordChanged = false;
//    //drawSteps();
//    system("killall monitor");
//    system("redirect_off.sh");
//    //mem.getSetting("ApCliWPAPSK", apPassword);
//
//    // do this after setup as RE
//    //    if (reconnect != 1)
//    //    {
//    //        if (ssidChoice == RECOMMENDED_SSID)
//    //        {
//    //            if (strlen(current.ssid) <= 25)
//    //                sprintf(ssid_ch, "%s_almond", current.ssid);
//    //            else
//    //                sprintf(ssid_ch, "%.*s_almond", 25, current.ssid);
//    //        }
//    //        else if (ssidChoice == SAME_SSID)
//    //        {
//    //            sprintf(ssid_ch, "%s", current.ssid);
//    //        }
//    //        else
//    //        {
//    //            sprintf(ssid_ch, "%ls", ssid);
//    //        }
//    //        mem.addValue("SSID1", ssid_ch);
//    //    }
//
//    if (isReconnecting != true) {
//        mem.getSetting("lan_ipaddr", oldLanIp);
//        mem.getSetting("dhcpEnd", oldDhcpEnd);
//        mem.getSetting("dhcpStart", oldDhcpStart);
//        mem.getSetting("dhcpGateway", oldDhcpGateway);
//        mem.getSetting("dhcpPriDns", oldDhcpDNS);
//
//        mem.addValue("old_lan_ipaddr", oldLanIp);
//        mem.addValue("old_dhcpEnd", oldDhcpEnd);
//        mem.addValue("old_dhcpStart", oldDhcpStart);
//        mem.addValue("old_dhcpGateway", oldDhcpGateway);
//        mem.addValue("old_dhcpPriDns", oldDhcpDNS);
//
//        oldLanIp[0] = '2';
//        oldDhcpStart[0] = '2';
//        oldDhcpEnd[0] = '2';
//        oldDhcpGateway[0] = '2';
//        oldDhcpDNS[0] = '2';
//
//        mem.addValue("lan_ipaddr", oldLanIp);
//        mem.addValue("dhcpEnd", oldDhcpEnd);
//        mem.addValue("dhcpStart", oldDhcpStart);
//        mem.addValue("dhcpGateway", oldDhcpGateway);
//        mem.addValue("dhcpPriDns", oldDhcpDNS);
//    }
//    mem.addValue("OperationMode", "3");
//    mem.addValue("ApCliWPAPSK", apPassword);
//
//    mem.addValue("AutoChannelSelect", "0");
//    mem.addValue("ApCliSsid", current.ssid);
//    mem.addValue("ApCliBssid", current.bssid);
//    mem.addValue("ApCliEnable", "1");
//    sprintf(output, "iwpriv ra0 set Channel=%s", current.channel);
//    system(output);
//    mem.addValue("Channel", current.channel);
//
//    if (current.authmode[0] == 'N' && current.authmode[1] == 'O') /*Open*/ {
//        mem.addValue("ApCliAuthMode", "OPEN");
//        mem.addValue("ApCliEncrypType", "NONE");
//        mem.addValue("AuthMode", "OPEN");
//        mem.addValue("EncrypType", "NONE");
//
//    } else if (current.authmode[1] == 'E') /*WEP*/ {
//        mem.addValue("ApCliAuthMode", "WEPAUTO");
//        mem.addValue("ApCliEncrypType", "WEP");
//        mem.addValue("ApCliDefaultKeyID", "1");
//
//        mem.addValue("ApCliKey1Str", apPassword);
//        mem.addValue("AuthMode", "WEPAUTO");
//        mem.addValue("EncrypType", "WEP");
//        int keyLen = strlen(apPassword);
//        if (keyLen == 5 || keyLen == 13) {
//            mem.addValue("ApCliKey1Type", "1");
//            mem.addValue("Key1Type", "1");
//        } else {
//            mem.addValue("ApCliKey1Type", "0");
//            mem.addValue("Key1Type", "0");
//        }
//        mem.addValue("Key1Str1", apPassword);
//        mem.addValue("Key1Str", apPassword);
//        mem.addValue("DefaultKeyID", "1");
//    } else /*WPA, WPA2, WPA/WPA2*/ {
//        // <editor-fold defaultstate="collapsed" desc="N-Mode Detection">
//
//        //int isNmode = 0;
//        //        int nCnt = 0;
//        //        while (unsigned(nCnt) < strlen(current.w_mode)) {
//        //            if (current.w_mode[nCnt] == 'n' || current.w_mode[nCnt] == 'N')
//        //                isNmode = 1;
//        //            nCnt++;
//        //        }
//        int isNmode = 0, nCnt = 0;
//        while (unsigned(nCnt) < strlen(current.w_mode)) {
//            if (current.w_mode[nCnt] == 'n' || current.w_mode[nCnt] == 'N')
//                isNmode = 1;
//            nCnt++;
//        }
//
//        // </editor-fold>
//
//        //If the current AuthMode is WPA1PSKWPA2PSK, fix it to WPAPSKWPA2PSK.
//        if (current.authmode[3] == '1')
//            strcpy(current.authmode + 3, current.authmode + 4);
//
//        //Save Current w_mode to nvram
//        mem.addValue("RepeaterMode", current.w_mode);
//
//        //Check for WPAPSKWPA2PSK
//        if (current.authmode[6] == 'W' && current.authmode[7] == 'P' && current.authmode[8] == 'A' && current.authmode[9] == '2') {
//            wpawpa2mode = 1;
//        }
//        //Check or TKIPAES
//        if (current.encryption[0] == 'T' && current.encryption[1] == 'K' && current.encryption[4] == 'A' && current.encryption[5] == 'E') {
//            tkipaesmode = 1;
//        }
//        //If WPAWPA2 Try WPA2 First
//        if (wpawpa2mode == 1) {
//            //drawTextWithBG(L"Debug: WPA/WPA2 detected. Trying WPA2 Mode First.", 0, 0, 319, 1, 0, 14.0f, 0, 65535, 1);
//            mem.addValue("ApCliAuthMode", "WPA2PSK");
//            wpa2try = 1;
//        } else {
//            mem.addValue("ApCliAuthMode", current.authmode);
//        }
//        //If TKIPAES, try AES First
//        if (tkipaesmode == 1) {
//            //drawTextWithBG(L"Debug: TKIPAES Detected. Trying AES Mode First.", 0, 13, 319, 1, 0, 14.0f, 0, 65535, 1);
//            mem.addValue("ApCliEncrypType", "AES");
//            aestry = 1;
//        } else {
//            mem.addValue("ApCliEncrypType", current.encryption);
//        }
//        if (isReconnecting == false) {
//            mem.addValue("AuthMode", current.authmode);
//            mem.addValue("EncrypType", current.encryption);
//            mem.getSetting("ApCliWPAPSK", apPassword);
//            //mem.addValue("WPAPSK1", apPassword);      //do this after setup as RE
//        }
//    }
//    mem.addValue("wanConnectionMode", "DHCP");
//    mem.saveValues();
//    //fillRect(16, 65, 16 + 11, 65 + 11, RGB(0, 255, 0));
//    system("internet.sh");
//    //fillRect(16, 95, 16 + 11, 95 + 11, RGB(0, 255, 0));
//    ip[0] = '<';
//    unsigned int timeoutCounter = 0;
//
//
//    while (ip[0] == '<' && (!(ip[0] >= '1' && ip[0] <= '9'))) {
//        if (timeoutCounter >= (60 * 2)) {
//            mem.saveSetting("OperationMode", "1");
//            system("internet.sh");
//
//            //mW.setLine1(lan.get("disconnectl1", "repeater"));
//            //mW.setLine2(lan.get("disconnectl2", "repeater"));
//            //mem.getSetting("ApCliSsid", output);
//            //swprintf(opt, 18, L"%hs", output);
//            //mW.setLine3(opt);
//            //mW.draw(MODAL_TYPE_CRITICAL, -1);
//            //lcd_clear(BACKGROUND_LAYER);
//
//            system("killall monitor");
//            system("monitor&");
//            return 0;
//        }
//        if (timeoutCounter >= (40)) {
//            if (wpa2try == 1) {
//                getIfIp("br0", ip);
//                if (ip[0] == '<' && (!(ip[0] >= '1' && ip[0] <= '9'))) {
//                    //drawTextWithBG(L"Debug: WPA2 Failed!. Trying WPA Mode.", 0, 26, 319, 1, 0, 14.0f, 0, 65535, 1);
//                    wpa2try = 0;
//                    mem.saveSetting("ApCliAuthMode", "WPAPSK");
//                    system("internet.sh");
//                }
//            }
//        }
//        if (timeoutCounter >= (60)) {
//            if (aestry == 1) {
//                getIfIp("br0", ip);
//                if (ip[0] == '<' && (!(ip[0] >= '1' && ip[0] <= '9'))) {
//                    //drawTextWithBG(L"Debug: AES Failed!. Trying TKIP Mode.", 0, 39, 319, 1, 0, 14.0f, 0, 65535, 1);
//                    aestry = 0;
//                    mem.addValue("ApCliEncrypType", "TKIP");
//                    system("internet.sh");
//                }
//            }
//        }
//        sleep(5);
//        //fillRect(32, 138 + 5, 32 + timeoutCounter, 151 + 5, 0);
//        timeoutCounter += 5;
//        getIfIp("br0", ip);
//    }
//    wpa2try = 0;
//    aestry = 0;
//    wpawpa2mode = 0;
//    tkipaesmode = 0;
//    //fillRect(32, 138 + 5, 150, 151 + 5, 0);
//    //fillRect(16, 125, 16 + 11, 125 + 11, RGB(0, 255, 0));
//    system("port_flapping_to_repeater.sh");
//    for (int tempVal = 0; tempVal < 10; tempVal++) {
//        if (checkConnection())
//            break;
//        else
//            sleep(1);
//    }
//    //fillRect(16, 155 + 10, 16 + 11, 155 + 11 + 10, RGB(0, 255, 0));
//    //update_settings = 1;
//    system("killall monitor");
//    system("monitor&");
//    return 1;
//}
//
//int setRangeExtender(ssinfo current, char *Password, bool isReconnecting) {
//    // ssinfo current;
//    // strcpy(current.channel, channel);
//    // strcpy(current.ssid, SSID);
//    // strcpy(current.bssid, BSSID);
//    // strcpy(current.authmode, AuthMode);
//    // strcpy(current.encryption, Encryption);
//    // strcpy(current.w_mode, w_mode);
//    printf("insetrangeextender\n");
//    printf("SSID %s BSSID %s Channel %s AuthMode %s,encryption %s WMode %s\n", current.ssid, current.bssid, current.channel, current.authmode, current.encryption, current.w_mode);
//    int ret = showSteps(current, Password, isReconnecting);
//    // if ret = 1 enable ssid3 and reboot 
//    if (ret == 1) {
//        mem.saveSetting("checkForSSID3", "1");
//        mem.saveSetting("enableSSID3", "1");
//        system("reboot");
//    } else {
//        mem.saveSetting("checkForSSID3", "1");
//        mem.saveSetting("enableSSID3", "0");
//    }
//    return ret;
//}
//
//void sendFirmwareResponse(bool success, int rc, int per) {
//    char resultStr[500];
//    if (per < 0) {
//        fw_thread = true;
//        snprintf(resultStr, 500, "{\"CommandType\":\"FirmwareUpdate\",\"Success\":\"%s\",\"ReasonCode\":\"%d\"}\n", success ? "true" : "false", rc);
//    } else {
//        snprintf(resultStr, 500, "{\"CommandType\":\"FirmwareUpdate\",\"Percentage\":\"%d\"}\n", per);
//    }
//    //       Addstatus_reply(resultStr);
//    printf("per %d\n", per); //swethaaaaaaaaaaaaaaaaaaaaaaaaaaa
//}
//
//void firmwareProcessing(int val) {
//    char tmp[100]; //, ch;
//    //    int i = 0;
//    static int startTime = time(0);
//    static int previosVal = val;
//    Logger *logger = Logger::getInstance();
//    int timerCount = time(0) - startTime;
//
//    if ((val % 10) == 0 && val != 1) {
//        if (previosVal != val) {
//            sprintf(tmp, "val=%hd", val);
//            LOG(Logger::INFO, tmp);
//            if (!downloadCancelled) {
//                sendFirmwareResponse(1, 0, val);
//            } else {
//                sendFirmwareResponse(0, 4, -1);
//            }
//
//            LOGINFO("After for loop after deleting userList");
//
//            previosVal = val;
//
//        }
//    }
//    if ((timerCount) == 3 * 60) {
//        LOG(Logger::WARN, "Its taking longer than usual");
//    }
//}
void getHueMAc_as_json(char *buf) {
    char mac[20], local_buf[50];
    //    list<Client>::iterator it;
    int i = 0;
    sprintf(buf, "[");

    while (!(strcmp(ARRAY[i], "FFFF") == 0)) {
        HueMac(ARRAY[i], mac);
        if (i > 0) {
            sprintf(local_buf, ",");
            strcat(buf, local_buf);
        }
        snprintf(local_buf, 50, "{\"id\":\"%s\"}", mac);
        strcat(buf, local_buf);

        i++;
    }
    sprintf(local_buf, "]");
    strcat(buf, local_buf);
}

void post_clients_data(int checkValue, struct libwebsocket *wsi, char *mii, struct json_token * arr) {
    int searchcheck = 1;
    unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + MAX_ECHO_PAYLOAD + LWS_SEND_BUFFER_POST_PADDING] = "";
    char* buffer = (char*) &buf[LWS_SEND_BUFFER_PRE_PADDING];
    switch (checkValue) {
        case CLIENTSLIST:
        {
           // if (WIRELESSMODE) {
           //     snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"ClientList\",\"Success\":\"false\",\"Clients\":\"[]\"}", mii);
		   //
           // } else {
                char localbuf[255000] = "";
                getClientList_as_json(localbuf);
                snprintf(buffer, 256000, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"ClientList\",\"Success\":\"true\",\"Clients\" : {%s}}", mii, localbuf);

           // }
        }
            break;




        case CLIENTUPDATE:
        {
            //            if (WIRELESSMODE) {
            //                snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            //            } else {
                char clientid[10], connection[10], presence[10], MAC[50], cname[200], client_type[25], wait[10], client[500], block[10], schedule[80] = "", reset[10] = "", fi[5] = "", UserType[34] = "", SMEnable[50] = "", BWEnable[50] = "", StaticIP[10], IP[16]="";
                struct json_token *cli = NULL;
                bool updatebandwidth;
                getJsonValue("Clients", client, arr, 500);
                cli = parse_json2(client, strlen(client));
                if ((getJsonValue("ID", clientid, cli, 10)) && (getJsonValue("Name", cname, cli, 200)) && (getJsonValue("Type", client_type, cli, 25)) && (getJsonValue("Connection", connection, cli, 10)) && (getJsonValue("UseAsPresence", presence, cli, 10)) && (getJsonValue("Wait", wait, cli, 10)) && (getJsonValue("MAC", MAC, cli, 50))) {
                    getJsonValue("Schedule", schedule, cli, 80);
                    getJsonValue("Block", block, cli, 10);
                    getJsonValue("ForceInactive", fi, cli, 10);
                    getJsonValue("Category", UserType, cli, 34);
                    getJsonValue("SMEnable", SMEnable, cli, 34);
                    getJsonValue("BWEnable", BWEnable, cli, 34);
                    getJsonValue("StaticIP", StaticIP, cli, 10);
                    getJsonValue("LastKnownIP", IP, cli, 16);
                    int clientID = 0;
                    memset(reset, 0, 10);
                    getJsonValue("ResetData", reset, cli, 10);
                    int iBlock = atoi(block);
                    clientID = atoi(clientid);
                    try{
                        strcpy(cname, decodestring(cname));
                        std::list<Client>::iterator it;
                        for (it = cl.clientsList.begin(); it != cl.clientsList.end(); ++it) {
                            if (strcmp(it->getMac(), MAC) == 0) {
                                if (((!strcmp(connection, "wired")) || (!strcmp(connection, "wireless")) || (!strcmp(connection, "unknown"))||(!strcmp(connection, "indirect"))) && ((!strcmp(presence, "true")) || (!strcmp(presence, "false")))) {
                                    if (it->setName(cname)) {
                                        if (it->setUserType(UserType)) {
                                        if (it->setType(stringToClientType(client_type))) {
                                            if (it->setConnectionType(connection)) {
                                                bool p;
                                                p = ((presence[0] == 'f' || presence[0] == 'F') ? 0 : 1);
                                                if (it->setUseAsPresence(p)) {
													if(getJsonValue("Block", block, cli, 10)){
                                                    it->setBlock(iBlock); 
													}
                                                 if (strlen(BWEnable) != 0) {
                                                            updatebandwidth = (BWEnable[0] == 'f' || BWEnable[0] == 'F') ? false : true;
                                                            it->setBWEnable(updatebandwidth);
                                                        }
                                                        if (strlen(SMEnable) != 0) {
                                                            updatebandwidth = (SMEnable[0] == 'f' || SMEnable[0] == 'F') ? false : true;
                                                            it->setSMEnable(updatebandwidth);
                                                        }
                                                     if(strlen(IP) != 0)
                                                        it->setIP(IP);
                                                    if (strlen(StaticIP) != 0) {
                                                        it->setStaticIP(strcmp(StaticIP,"true")==0 ? 1:0);
                                                    }
                                                    if (strlen(schedule) != 0)
                                                        it->setSchedule(schedule);
                                                    if (strlen(fi) != 0)
                                                        it->setForceInactive(atoi(fi));
                                                    if (it->setWait(atoi(wait)))
                                                        snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
                                                    else
                                                        //wait
                                                        snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"Wait\"}", mii);
                                                } else {
                                                    //presence
                                                    snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"UseAsPresence\"}", mii);
                                                }
                                            } else {
                                                // connection
                                                snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"Connection\"}", mii);
                                            }
                                        } else {
                                            //type failed
                                            snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"Type\"}", mii);
                                        }
                                            snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);

                                        } else {
                                            snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"Category\"}", mii);
                                        }
                                    } else {
                                        //name failed
                                        snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"Name\"}", mii);
                                    }
                                } else {
                                    // wired and true
                                    snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                                }
                                if (reset[0] == '1') {
                                    it->setRXTX(0, 0, 1);
                                }
                                Event e;
                                e.event = ClientUpdated;
                                ClientEvent ce;
                                ce.ClientID = clientID;
                                ce.active = it->isActive();
                                strcpy(ce.mac, it->getMac());
                                strcpy(ce.ip, it->getIP());
                                memcpy(e.data, &ce, sizeof (ce));
                                HADevices::broadcastEvent(e);
                                break;

                            }
                        }
                        if (it == cl.clientsList.end()) {
                            snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                        }
                    } catch (int i) {
                        snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                    }
                } else {
                    snprintf(buffer, 500, "{\"CommandType\":\"UpdateClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                }
                if (cli) {
                free(cli);
                cli = NULL;
            }
        //}
        }
            break;
        case REMOVE_CLIENT:
        {
            //            if (WIRELESSMODE) {
            //                snprintf(buffer, 500, "{\"CommandType\":\"RemoveClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            //            } else {
                char clientid[10], MAC[50];
                struct json_token *sc = NULL;
                sc = find_json_token(arr, "Clients");
                if (sc != NULL) {
                    if ((getJsonValue("ID", clientid, sc, 10)) && (getJsonValue("MAC", MAC, sc, 50))) {
                        int clientID = 0;
                        clientID = atoi(clientid);
                        try{
                            std::list<Client>::iterator it;
                            for (it = cl.clientsList.begin(); it != cl.clientsList.end(); ++it) {
                                if (strcmp(it->getMac(), MAC) == 0) {
                                    it->deleteClient();
                                    snprintf(buffer, 500, "{\"CommandType\":\"RemoveClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
                                    Event e;
                                    e.event = ClientRemoved;
                                    ClientEvent ce;
                                    ce.ClientID = clientID;
                                    strcpy(ce.mac, MAC);
                                    memcpy(e.data, &ce, sizeof (ce));
                                    HADevices::broadcastEvent(e);
                                    break;
                                }
                            }
                            if (it == cl.clientsList.end()) {
                                snprintf(buffer, 500, "{\"CommandType\":\"RemoveClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                            }

                        } catch (int i) {
                            snprintf(buffer, 500, "{\"CommandType\":\"RemoveClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                        }
                    } else {
                        snprintf(buffer, 500, "{\"CommandType\":\"RemoveClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                    }
                } else {
                    snprintf(buffer, 500, "{\"CommandType\":\"RemoveClient\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                }
                if (sc) {
                free(sc);
                sc = NULL;
            }
      //  }
        }
            break;
        case REMOVE_ALL_CLIENTS:
        {
     		//            if (WIRELESSMODE) {
            //                snprintf(buffer, 500, "{\"CommandType\":\"RemoveAllClients\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            //            } else {

                cl.removeAllClients();
                snprintf(buffer, 500, "{\"CommandType\":\"RemoveAllClients\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
                Event e;
                e.event = ClientRemoveAll;
                HADevices::broadcastEvent(e);
           // }
        }
            break;
        case SCENES_LIST:
        {
            char JSON[255000] = "";
            getSceneList_as_json(JSON);
            snprintf(buffer, 256000, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"DynamicSceneList\",\"Scenes\" : {%s}}", mii, JSON);
        }
            break;
        case CREATE_SCENE:
        {
            //printf("createscene\n");
            int array_size;
            int i = 0;
            bool response = false;
            char devid[10], index[10], val[50], sss[100], List[4096], scenename [200], Voice[10] = "";
            struct json_token *sc = NULL;
            sc = find_json_token(arr, "Scenes");
            if (sc != NULL) {
                if ((getJsonValue("Name", scenename, sc, 200)) &&(getJsonValue("SceneEntryList", List, sc, 4096))) {
                    struct json_token *st = NULL;
                    st = find_json_token(sc, "SceneEntryList");
                    strcpy(scenename, decodestring(scenename));
                    Scene s(scenename);
					if (getJsonValue("VoiceCompatible", Voice, sc, 200)) {
                    	if (strcmp(Voice, "true") == 0) {
                    		s.setSceneVoiceCompatible(TRUE);
                    	} else {
                    	    s.setSceneVoiceCompatible(FALSE);
                    	}
                    }
                    array_size = st->num_desc / 7;

                    for (i = 0; i < array_size; i++) {
                        sprintf(sss, "SceneEntryList[%d]", i);
                        st = find_json_token(sc, sss);

                        if ((getJsonValue("ID", devid, st, 10)) &&(getJsonValue("Index", index, st, 10)) &&(getJsonValue("Value", val, st, 50))) {
                            response = AddDevicetoscene(s.getID(), atoi(devid), atoi(index), val);
                        }

                        if (st) {
                        free(st);
                        st = NULL;
                        }
                    }
                    if (response) {
                        LOG(Logger::INFO, "failed Deleting the Scene");
                        s.deleteScene();
                        snprintf(buffer, 500, "{\"CommandType\":\"AddScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                    } else {
                        LOG(Logger::INFO, "Added the Scene");
                        snprintf(buffer, 500, "{\"CommandType\":\"AddScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
                        Event e;
                        e.event = SceneAdded;
                        ScenesEvent se;
                        se.SceneId = s.getID();
                        memcpy(e.data, &se, sizeof (se));
                        HADevices::broadcastEvent(e);
                    }
                    array_size = 0;
                    //Persist(1,"/tmp/sdb");
                }
                if (sc) {
                free(sc);
                sc = NULL;
                }
            } else {
                snprintf(buffer, 500, "{\"CommandType\":\"AddScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            }
        }
            break;
        case SET_SCENE:
        {
            int array_size;
            int i = 0, SID;
            //            bool response = false;
            bool response = false;
            char devid[10], index[10], val[50], sceneid[10], scenename[200], sss[100], List[4096], Voice[10] = "";
            struct json_token *sc = NULL;
            sc = find_json_token(arr, "Scenes");
            if (sc != NULL) {
                if ((getJsonValue("Name", scenename, sc, 200)) &&(getJsonValue("SceneEntryList", List, sc, 4096)) &&(getJsonValue("ID", sceneid, sc, 10))) {
                    struct json_token *st = NULL, *list = NULL;
                    list = find_json_token(sc, "SceneEntryList");
                    array_size = list->num_desc / 7;
                    SID = atoi(sceneid);
                    if (list) {
                    free(list);
                    list = NULL;
                    }
                    try{
                        std::list<Scene>::iterator it;
                        for (it = sl.Scenes.begin(); it != sl.Scenes.end(); ++it) {
                            if (SID == it->getID()) {
                                it->removeAllSceneEntrys();
                                strcpy(scenename, decodestring(scenename));
                                it->setName(scenename);
                                if (getJsonValue("VoiceCompatible", Voice, sc, 10)) {
                                    if (strcmp(Voice, "true") == 0) {
                                        it->setSceneVoiceCompatible(TRUE);
                                    } else {
                                        it->setSceneVoiceCompatible(FALSE);
                                    }
                                }
                                for (i = 0; i < array_size; i++) {
                                    sprintf(sss, "SceneEntryList[%d]", i);
                                    st = find_json_token(sc, sss);
                                    if ((getJsonValue("ID", devid, st, 10)) &&(getJsonValue("Index", index, st, 10)) &&(getJsonValue("Value", val, st, 50))) {
                                        //response = 
                                        response = AddDevicetoscene(it->getID(), atoi(devid), atoi(index), val);
                                    }
                                    if (st) {
                                    free(st);
                                    st = NULL;
                                }
                                }
                               // if (response) {
                                //    it->deleteScene();
                                //    snprintf(buffer, 500, "{\"CommandType\":\"UpdateScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                               // } else {
                                    snprintf(buffer, 500, "{\"CommandType\":\"UpdateScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
                                    Event e;
                                    e.event = SceneUpdated;
                                    ScenesEvent se;
                                    se.SceneId = it->getID();
                                    memcpy(e.data, &se, sizeof (se));
                                    HADevices::broadcastEvent(e);

                               // }
                                array_size = 0;
                                break;
                            }
                        }
                        if (it == sl.Scenes.end()) {
                            snprintf(buffer, 500, "{\"CommandType\":\"UpdateScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
                        }
                    } catch (int SID) {
                        snprintf(buffer, 500, "{\"CommandType\":\"UpdateScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                    }
                    //Persist(1,"/tmp/sdb");

                }
                if (sc) {
                free(sc);
                sc = NULL;
                }
            } else {
                snprintf(buffer, 500, "{\"CommandType\":\"UpdateScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            }
        }
            break;
        case ACTIVATE_SCENE:
        {
            char sceneid[10];
            int s_id;
            struct json_token *sc = NULL;
            sc = find_json_token(arr, "Scenes");
            if (sc != NULL) {
                if (getJsonValue("ID", sceneid, sc, 10)) {
                    s_id = atoi(sceneid);
                    try{
                        //                    printf("activeate scene\n");
                        std::list<Scene>::iterator it;
                        for (it = sl.Scenes.begin(); it != sl.Scenes.end(); ++it) {
                            if (s_id == it->getID()) {
                                it->activate();
                                snprintf(buffer, 500, "{\"CommandType\":\"ActivateScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
                                //                            Event e;
                                //                            e.event = SceneActivated;
                                //                            ScenesEvent se;
                                //                            se.SceneId = it->getID();
                                //                            memcpy(e.data, &se, sizeof (se));
                                //                            HADevices::broadcastEvent(e);
                                break;
                            }
                        }
                        if (it == sl.Scenes.end()) {
                            snprintf(buffer, 500, "{\"CommandType\":\"ActivateScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
                        }
                    } catch (int s_id) {
                        snprintf(buffer, 500, "{\"CommandType\":\"ActivateScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                    }
                }
                if (sc) {
                free(sc);
                sc = NULL;
                }
            } else {
                snprintf(buffer, 500, "{\"CommandType\":\"ActivateScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            }
        }
            break;
        case REMOVE_SCENE:
        {
            char sceneid[10];
            int s_id;
            struct json_token *sc = NULL;
            sc = find_json_token(arr, "Scenes");
            if (sc != NULL) {
                if (getJsonValue("ID", sceneid, sc, 10)) {
                    s_id = atoi(sceneid);
                    try{
                        std::list<Scene>::iterator it;
                        for (it = sl.Scenes.begin(); it != sl.Scenes.end(); ++it) {
                            if (s_id == it->getID()) {
                                it->deleteScene();
                                snprintf(buffer, 500, "{\"CommandType\":\"RemoveScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
                                Event e;
                                e.event = SceneRemoved;
                                ScenesEvent se;
                                se.SceneId = it->getID();
                                memcpy(e.data, &se, sizeof (se));
                                HADevices::broadcastEvent(e);
                                break;
                            }
                        }

                        if (it == sl.Scenes.end()) {
                            snprintf(buffer, 500, "{\"CommandType\":\"RemoveScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
                        }
                    } catch (int s_id) {
                        snprintf(buffer, 500, "{\"CommandType\":\"RemoveScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                    }
                    //Persist(1,"/tmp/sdb");
                } else {
                    snprintf(buffer, 500, "{\"CommandType\":\"RemoveScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                }
                if (sc) {
                free(sc);
                sc = NULL;
                }
            } else {
                snprintf(buffer, 500, "{\"CommandType\":\"RemoveScene\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            }
        }
            break;
        case REMOVE_ALL_SCENES:
        {
            sl.deleteAllScenes();
            //Persist(1,"/tmp/sdb");
            snprintf(buffer, 500, "{\"CommandType\":\"RemoveAllScenes\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
            Event e;
            e.event = SceneRemoveAll;
            HADevices::broadcastEvent(e);
        }
            break;
        case SET_ALMOND_NAME:
        {
            bool r;
            char almondname[200];
            if (getJsonValue("Name", almondname, arr, 200)) {
                r = al.setAlmondName(almondname);
                if (!r) { //failure
                    snprintf(buffer, 500, "{\"CommandType\":\"SetAlmondName\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
                } else { //success
                    snprintf(buffer, 500, "{\"CommandType\":\"SetAlmondName\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
                }
            }
        }
            break;







case DISPLAY:
        {
          printf("hiiiiiiii\n");
                char result[100];
                char S[100]="./scenes ";
               if(getJsonValue("value", result, arr, 200))
                {
                  strcat(S,result);
                  printf("S is:%s\n", S);
                  system(S);
                 }          
        }
         break;







        case GET_MAC_NAME:
        {
            char MAC[50];
            getIfMac("ra0", MAC);
            snprintf(buffer, 500, "{\"CommandType\":\"GetAlmondNameandMAC\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Name\":\"%s\",\"MAC\":\"%s\",\"Almond\":\"AL3\"}", mii, ALMONDNAME, MAC);
        }
            break;
        case ROUTER_SUMMARY://new
        {
            char srbuf[1024], slave[100];
            char buff[2048] = "";
            routersummary(srbuf);
                  
            ml.Populate();
            list<Mesh>::iterator it;
            int i, j = 0;
            for (i = 0, it = ml.meshSlaveList.begin(); it != ml.meshSlaveList.end(); ++it, ++i) {
                if (j > 0) {
                    sprintf(slave, ",");
                    strcat(buff, slave);
                }
                if ((it->getSlavePosID()) != 0) {
                    j++;
                    snprintf(slave, 100, "{\"SlaveUniqueName\":\"%s\",\"Name\":\"%s\",\"Location\":\"%s\",\"HopCount\":\"%d\"}", it->getSlaveUniqueName(), it->getName(), it->getLocation(), it->getHopCount());
                    strcat(buff, slave);
                }

            }
            snprintf(buffer, 2048, "{\"CommandType\":\"RouterSummary\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Slaves\":[%s],%s}", mii, buff, srbuf);
        }
            break;
            //        case GET_WIRELESS_SETTINGS:
            //        {
            //            char srbuf[4096]; //, enc_buf[4096];
            //            GetrouterwirelessSettings(srbuf);
            //            //            b64_encode(srbuf, enc_buf);
            //            snprintf(buffer, 5120, "{\"CommandType\":\"GetWirelessSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",%s}", mii, srbuf);
            //        }
            //            break;
            //        case SET_WIRELESS_SETTINGS:
            //        {
            //            //char WirelessData[1024], R_type[180], SSID[180], PASSWD[180], UPTIME[180], ENABLED[20];
            //			char WirelessData[1024], R_type[180], SSID[180], ENABLED[20];
            //            char srbuf[4096];
            //            //if (getJsonValue("WirelessSetting", WirelessData, arr, 1024)&& (getJsonValue("UpTime", UPTIME, arr, 180))) {
            //                struct json_token  *wd;//,*rd;
            //                //wd = parse_json2(WirelessData, strlen(WirelessData));
            //                WirelessData[0] = '\0';
            //                //  printf("WirelessData - %s\n",WirelessData);
            //                if (getJsonValue("WirelessSetting", WirelessData, arr, 1024)) {
            //                    wd = parse_json2(WirelessData, strlen(WirelessData));
            //                if (getJsonValue("Type", R_type, wd, 180)) { //Broad cast
            //                    if ((getJsonValue("SSID", SSID, wd, 180))) {
            //                        //             {"CommandType":"GetRouterSummary","MobileInternalIndex":"123","Success":"true","Reason":"0","wireless":{"5G":{"SSID":"Almond+5g","enabled":"1"},"2G":{"SSID":"Almond+2g","enabled":"1"},"Guest5G":{"SSID":"guest+5g","enabled":"1"},"Guest2G":{"SSID":"guest+2g","enabled":"1"}}}
            //                      //  if (getJsonValue("Password", PASSWD, wd, 180)) {
            //                            if (getJsonValue("Enabled", ENABLED, wd, 20)) {
            //                               // if (SetIDandPassword(R_type, SSID, PASSWD, UPTIME, ENABLED)) {
            //                            SetIDandPassword(R_type, SSID, ENABLED, srbuf);
            //                            //, enc_buf[4096];
            //                            //                                GetrouterwirelessSettings(srbuf);
            //                            snprintf(buffer, 5120, "{\"MobileInternalIndex\":\"%s\",%s}", mii, srbuf);
            //                                //            printf("buffer- %s\n", routersummary().c_str());
            //                            } else { // enabled
            //                                //            snprintf(buffer, 500, "{\"CommandType\":\"GetRouterSummary\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",%s}", mii, routersummary().c_str());
            //                                snprintf(buffer, 500, "{\"CommandType\":\"SetWirelessSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            //                            }
            //                       // } else { // pswd
            //                       //     snprintf(buffer, 500, "{\"CommandType\":\"SetWirelessSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            //                      //  }
            //                    } else { //ssid
            //                        snprintf(buffer, 500, "{\"CommandType\":\"SetWirelessSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            //                    }
            //                } else {//Type
            //                    snprintf(buffer, 500, "{\"CommandType\":\"SetWirelessSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            //                }
            //            } else {//Wireless
            //                snprintf(buffer, 500, "{\"CommandType\":\"SetWirelessSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"110\"}", mii);
            //            }
            //            /*} else {// Data
            //                snprintf(buffer, 500, "{\"CommandType\":\"SetWirelessSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            //            }*/
            //        }
            //            break;
            //        case ROUTER_REBOOT:
            //        {
            //            snprintf(buffer, 500, "{\"CommandType\":\"RebootRouter\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
            //            system("reboot");
            //        }
            //            break;
            //        case SEND_LOGS:
            //        {
            //            char Message[1024];
            //            int ret = 0;
            //            if (getJsonValue("Message", Message, arr, 1024)) {
            //                SendLogs *send = new SendLogs();
            //                send->setMessage(Message);
            //                ret = send->mainSendLogs();
            //                if (ret) {
            //                    snprintf(buffer, 500, "{\"CommandType\":\"SendLogs\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"%d\"}", mii, ret);
            //                } else {
            //                    snprintf(buffer, 500, "{\"CommandType\":\"SendLogs\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"%d\"}", mii, ret);
            //                }
            //            } else {
            //                snprintf(buffer, 500, "{\"CommandType\":\"SendLogs\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"%d\"}", mii, ret);
            //            }
            //        }
            //            break;
            //        case FIRMWARE_UPDATE:
            //        {
            //            printf("firmware Update\n");
            //            if (fw_thread) {
            //                fw_thread = false;
            //                addwsi = wsi;
            //                Firmware *firmware;
            //                char Version[1024];
            //                try{
            //                    LOG(Logger::PRINT, "MD5 .\n");
            //                    if (getJsonValue("Version", Version, arr, 1024)) {
            //                        //printf("firmware Version %s  ---\n",Version);
            //                        firmware = new Firmware((char *) Version, true);
            //                        LOG(Logger::PRINT, "MD5 downloaded.\n");
            //                        if (firmware->downloadUpdateFirmware()) {
            //                            LOG(Logger::PRINT, "MD5 matched ok");
            //
            //                            sendFirmwareResponse(1, 10, -1);
            //                        } else {
            //                            sendFirmwareResponse(0, 2, -1);
            //                            //                        printf("FAILLLLLLLLLLLLLLLLLLLLLLLll\n");
            //                        }
            //                    }
            //                } catch (int e) {
            //                    printf("firmware fail to update\n");
            //                    LOG(Logger::ERROR, "Error in Download");
            //                    //                    sendFirmwareResponse(0, 3, -1);
            //                }
            //            } else {
            //                printf("{\"CommandType\":\"FirmwareUpdate\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"ReasonCode\":\"0\"}\n", mii);
            //            }
            //
            //        }
            //            break;
            //        case ALMOND_PROPERTIES:
            //        {
            //            char srbuf[1024];
            //            AlmondProperties(srbuf);
            //            snprintf(buffer, 1024, "{\"CommandType\":\"AlmondProperties\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",%s}", mii, srbuf);
            //        }
            //            break;
        case ASSOC_GET_NAME:
        {
            char Name[200] = "", Location[200] = "";
            if (!th_1) {
                if(getJsonValue("Location", Location, arr, 200)) {
                    alAssoc->setDevLocation(decodestring(Location));
                }
                if (getJsonValue("Name", Name, arr, 200)) {
                    alAssoc->setDevName(decodestring(Name));
                }
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"GetName\",\"Success\":\"true\"}", mii);
                alAssoc->addingDone();
            } else {
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"GetName\",\"Success\":\"false\"}", mii);
            }
        }
            break;
             //        case MEMORYCLASS:
            //        {
            //            char method[180];
            //            int memclass;
            //            if (getJsonValue("Method", method, arr, 180)) {
            //                memclass = getmemoryfunction(method);
            //                switch (memclass) {
            //                    case GET_SETTING:
            //                    case GET_NOSAVESETTING:
            //                    {
            //                        char variable[255], count[10], output[180], list[1024], sss[100], buf[512], JSON[4096], localbuf[3];
            //                        int i;
            //
            //                        if ((getJsonValue("VariableCount", count, arr, 10))&&(getJsonValue("VariableList", list, arr, 1024))) {
            //                            struct json_token *st = NULL, *ss = NULL;
            //                            st = find_json_token(arr, "VariableList");
            //                            sprintf(JSON, "[");
            //                            for (i = 0; i < atoi(count); i++) {
            //                                sprintf(sss, "VariableList[%d]", i);
            //                                ss = find_json_token(arr, sss);
            //                                getJsonValue("Variable", variable, ss, 255);
            //                                mem.getSetting(variable, output);
            //                                //                                LOGINFO(sl_str.c_str());
            //                                getvariableswithvalue_array(variable, output, buf);
            //                                if (i > 0) {
            //                                    sprintf(localbuf, ",");
            //                                    strcat(JSON, localbuf);
            //                            }
            //                                strcat(JSON, buf);
            //                                //                                sst << sl_str;
            //                            }
            //                            sprintf(localbuf, "]");
            //                            strcat(JSON, localbuf);
            //                            //                            sst << "]";
            //                            snprintf(buffer, 2048, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Method\":\"getSetting\",\"VariableCount\":\"%s\",\"VariableList\":%s}", mii, count, JSON);
            //                            if (st) {
            //                            free(st);
            //                            st = NULL;
            //                            }
            //                            if (ss) {
            //                            free(ss);
            //                            ss = NULL;
            //                            }
            //                        } else {
            //                            snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\",\"Method\":\"getSetting\"}", mii);
            //                        }
            //                    }
            //                        break;
            //
            //                        //                    {
            //                        //                        char variable[255], count[10], output[180], list[1024], sss[100];
            //                        //                        int i;
            //                        //
            //                        //                        if ((getJsonValue("VariableCount", count, arr))&&(getJsonValue("VariableList", list, arr))) {
            //                        //                            struct json_token *st, *ss;
            //                        //                            st = find_json_token(arr, "VariableList");
            //                        //                            std::stringstream sst;
            //                        //                            sst << "[";
            //                        //                            for (i = 0; i < atoi(count); i++) {
            //                        //                                sprintf(sss, "VariableList[%d]", i);
            //                        //                                ss = find_json_token(arr, sss);
            //                        //                                getJsonValue("Variable", variable, ss);
            //                        //                                mem.getNoSaveSetting(variable, output);
            //                        //                                std::string sl_str = getvariableswithvalue_array(variable, output);
            //                        //                                if (i > 0)
            //                        //                                    sst << COMMA;
            //                        //                                //                                LOGINFO(sl_str.c_str());
            //                        //                                sst << sl_str;
            //                        //                            }
            //                        //                            sst << "]";
            //                        //                            snprintf(buffer, 2048, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Method\":\"getNoSaveSetting\",\"VariableCount\":\"%s\",\"VariableList\":%s}", mii, count, sst.str().c_str());
            //                        //                        } else {
            //                        //                            snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\",\"Method\":\"getNoSaveSetting\"}", mii);
            //                        //                        }
            //                        //                    }
            //                        //                        break;
            //                    case SAVE_SETTING:
            //                    {
            //                        char variable[255], value[180];
            //                        if (getJsonValue("Variable", variable, arr, 255) && getJsonValue("Value", value, arr, 180)) {
            //                            mem.saveSetting(variable, value);
            //                            snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Method\":\"saveSetting\",\"Variable\":\"%s\",\"Value\":\"%s\"}", mii, variable, value);
            //                        } else {
            //                            snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\",\"Method\":\"saveSetting\"}", mii);
            //                        }
            //                    }
            //                        break;
            //                        //                    case ADD_CONFIG:
            //                        //                    {
            //                        //                        char config[180], sectiontype[180];
            //                        //                        if (getJsonValue("Config", config, arr) && getJsonValue("SectionType", sectiontype, arr)) {
            //                        //                            mem.addConfig(config, sectiontype);
            //                        //                            snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Method\":\"addConfig\",\"Config\":\"%s\",\"SectionType\":\"%s\"}", mii, config, sectiontype);
            //                        //                        } else {
            //                        //                            snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\",\"Method\":\"addConfig\"}", mii);
            //                        //                        }
            //                        //                    }
            //                        //                        break;
            //                        //                    case DEL_SETTING:
            //                        //                    {
            //                        //                        char variable[255];
            //                        //                        if (getJsonValue("Variable", variable, arr)) {
            //                        //                            mem.delSetting(variable);
            //                        //                            snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Method\":\"delSetting\",\"Variable\":\"%s\"}", mii, variable);
            //                        //                        } else {
            //                        //                            snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\",\"Method\":\"delSetting\"}", mii);
            //                        //                        }
            //                        //                    }
            //                        //                        break;
            //                    case ADD_VALUE:
            //                    {
            //                        char variable[255], count[10], output[180], list[1024], sss[100], buf[512], JSON[4096], localbuf[3];
            //                        int i;
            //
            //                        if ((getJsonValue("VariableCount", count, arr, 10))&&(getJsonValue("VariableList", list, arr, 1024))) {
            //                            struct json_token *st = NULL, *ss = NULL;
            //                            st = find_json_token(arr, "VariableList");
            //                            sprintf(JSON, "[");
            //                            for (i = 0; i < atoi(count); i++) {
            //                                sprintf(sss, "VariableList[%d]", i);
            //                                ss = find_json_token(arr, sss);
            //                                getJsonValue("Variable", variable, ss, 255);
            //                                getJsonValue("Value", output, ss, 180);
            //                                mem.addValue(variable, output);
            //                                //                                LOGINFO(sl_str.c_str());
            //                                getvariableswithvalue_array(variable, output, buf);
            //                                if (i > 0) {
            //                                    sprintf(localbuf, ",");
            //                                    strcat(JSON, localbuf);
            //                            }
            //                                strcat(JSON, buf);
            //                                //                                sst << sl_str;
            //                            }
            //                            //                            sst << "]";
            //                            sprintf(localbuf, "]");
            //                            strcat(JSON, localbuf);
            //                            mem.saveValues();
            //                            snprintf(buffer, 2048, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Method\":\"addValue\",\"VariableCount\":\"%s\",\"VariableList\":%s}", mii, count, JSON);
            //                            if (st) {
            //                            free(st);
            //                            st = NULL;
            //                            }
            //                            if (ss) {
            //                            free(ss);
            //                            ss = NULL;
            //                            }
            //                        } else {
            //                            snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\",\"Method\":\"addvalue\"}", mii);
            //                        }
            //                    }
            //                        break;
            //                    case SAVE_VALUE:
            //                    {
            //                        mem.saveValues();
            //                        snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Method\":\"saveValues\"}", mii);
            //                    }
            //                        break;
            //                    case COMMIT:
            //                    {
            //                        mem.commit();
            //                        snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Method\":\"commit\"}", mii);
            //                    }
            //                        break;
            //                        //                    case CREATE_SETTING_FILE:
            //                        //                    {
            //                        //                        char filename[180];
            //                        //                        if (getJsonValue("Filename", filename, arr)) {
            //                        //                            mem.createSettingsFile(filename);
            //                        //                            snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Method\":\"createSettingsFile\",\"Filename\":\"%s\"}", mii, filename);
            //                        //                        } else {
            //                        //                            snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\",\"Method\":\"createSettingsFile\"}", mii);
            //                        //                        }
            //                        //                    }
            //                        //                        break;
            //                    case GETMAC:
            //                    {
            //                        char output[180];
            //                        mem.getMac(output);
            //                        snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Method\":\"getMac\",\"Value\":\"%s\"}", mii, output);
            //
            //                    }
            //                        break;
            //                    case GETCLOUDID:
            //                    {
            //                        char output[180];
            //                        mem.getCloudID(output);
            //                        snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Method\":\"getCloudID\",\"Value\":\"%s\"}", mii, output);
            //
            //                    }
            //                        break;
            //                    case 0:
            //                    default:
            //                        snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Method\":\"UnKnownMethod\"}", mii);
            //                        break;
            //                }
            //
            //            } else {
            //                snprintf(buffer, 500, "{\"CommandType\":\"RouterSettings\",\"MobileInternalIndex\":\"%s\",\"Method\":\"UnKnownMethod\"}", mii);
            //            }
            //        }
            //            break;
            //        case SYSTEMCOMMAND:
            //        {
            //            FILE *fptr;
            //            char input[100], output[102048] = "";
            //            if (getJsonValue("Input", input, arr, 100)) {
            //                fptr = popen(input, "r");
            //                fread(output, 1, sizeof (output), fptr);
            //                snprintf(buffer, 102400, "{\"CommandType\":\"SystemCommand\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Input\":\"%s\",\"Output\":\"%s\"}", mii, input, output);
            //                pclose(fptr);
            //
            //            }
            //        }
            //            break;
            //        case SITE_SURVEY:
            //        {
            //            // ssinfo ss[100];
            //            unsigned char ss[50000];
            //            return_site_survey_buffer(ss);
            //            snprintf(buffer, 102400, "{\"CommandType\":\"SiteSurvey\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Output\":\"%s\"}", mii, ss);
            //        }
            //            break;
            //        case CHECK_INTERNET:
            //        {
            //            snprintf(buffer, 500, "{\"CommandType\":\"CheckInternet\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Output\":\"%d\"}", mii, checkConnection());
            //        }
            //            break;
            //        case CHECK_CABLES:
            //        {
            //            snprintf(buffer, 500, "{\"CommandType\":\"CheckCables\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Output\":\"%03d\"}", mii, checkCables());
            //        }
            //            break;
            //        case CHECK_WAN_CABLE:
            //        {
            //            snprintf(buffer, 500, "{\"CommandType\":\"CheckWANCable \",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Output\":\"%d\"}", mii, checkWANCable());
            //        }
            //            break;
            //        case CHECK_CABLE_INTERNET:
            //        {
            //            snprintf(buffer, 500, "{\"CommandType\":\"CheckCableAndInternet\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Output\":\"%d\"}", mii, checkCableAndInternet());
            //        }
            //            break;
            //        case GET_SSID_AND_PASSWORD:
            //        {
            //            //snprintf(buffer, 500, "{\"CommandType\":\"CheckCableAndInternet\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Output\":\"%d\"}", mii, getSSIDAndPassword());
            //        }
            //            break;
            //        case SET_SSID_AND_PASSWORD:
            //        {
            //            //snprintf(buffer, 500, "{\"CommandType\":\"CheckCableAndInternet\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Output\":\"%d\"}", mii, setSSIDAndPassword());
            //        }
            //            break;
            //        case ENABLE_RANGE_EXTENDER:
            //        {
            //            //SetRangeExtender
            //            char isreconnecting[20];
            //            char apPassword[180];
            //            ssinfo current;
            //            //            {"CommandType":"SetRangeExtender","MobileInternalIndex":"123","SSID":"cdejvc","BSSID":"defnkc","AuthMode":"hfh","Encryption":"dekhd","WMode":"123","Channel":"1","isReconnect":"12"}
            //
            //            if ((getJsonValue("SSID", current.ssid, arr, 50))&&(getJsonValue("Password", apPassword, arr, 180))&&(getJsonValue("BSSID", current.bssid, arr, 50))&&(getJsonValue("AuthMode", current.authmode, arr, 15))&&(getJsonValue("Encryption", current.encryption, arr, 10)) && (getJsonValue("WMode", current.w_mode, arr, 8)) && (getJsonValue("Channel", current.channel, arr, 3)) && (getJsonValue("isReconnecting", isreconnecting, arr, 20))) {
            //
            //                snprintf(buffer, 500, "{\"CommandType\":\"EnableRangeExtender\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
            //                libwebsocket_write(wsi, (unsigned char *) buffer, strlen(buffer), LWS_WRITE_TEXT);
            //                buffer = "";
            //                if (isreconnecting[0] == 't' || isreconnecting[0] == '1')
            //                    showSteps(current, apPassword, 1);
            //                else
            //                    showSteps(current, apPassword, 0);
            //            } else {
            //                snprintf(buffer, 500, "{\"CommandType\":\"EnableRangeExtender\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            //            }
            //
            //            //            snprintf(buffer, 500, "{\"CommandType\":\"CheckCableAndInternet\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Output\":\"%d\"}", mii, setSSIDAndPassword());
            //        }
            //            break;
            //        case DISABLE_RANGE_EXTENDER:
            //        {
            //            snprintf(buffer, 500, "{\"CommandType\":\"DisableRangeExtender\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
            //            libwebsocket_write(wsi, (unsigned char *) buffer, strlen(buffer), LWS_WRITE_TEXT);
            //            buffer = "";
            //            disable_rangeExtender();
            //        }
            //            break;
            //
            //        case ENABLE_SSID3:
            //        {
            //            char SSID3[32], output[180];
            //            if (getJsonValue("SSID3", SSID3, arr, 32)) {
            //                mem.addValue("enableSSID3", "1");
            //                mem.getSetting("BssidNum", output);
            //                if (atoi(output) < 3)
            //                    mem.addValue("BssidNum", "3");
            //                mem.getSetting("SSID2", output);
            //                srandom(time(NULL));
            //                if (strlen(output) == 0) {
            //                    sprintf(output, "Guest%04ld", random() % 10000);
            //                    mem.addValue("SSID2", output);
            //                }
            //                mem.addValue("SSID3", SSID3);
            //
            //                mem.getSetting("WPAPSK2", output);
            //                if (strlen(output) == 0) {
            //                    sprintf(output, "12345678%04ld", random() % 10000);
            //                    mem.addValue("WPAPSK2", output);
            //                }
            //
            //                bzero(output, 180);
            //                mem.getSetting("HideSSID", output);
            //                if (strcmp(output, "0") == 0) {
            //                    mem.addValue("HideSSID", "0;1;0");
            //                } else {
            //                    output[1] = ';';
            //                    if (!(output[2] == '0' || output[2] == '1'))
            //                        output[2] = '1';
            //                    output[3] = ';';
            //                    output[4] = '0';
            //                    mem.addValue("HideSSID", output);
            //                }
            //
            //                mem.getSetting("AuthMode", output);
            //                if (strchr(output, ';') == NULL) {
            //                    strcat(output, ";WPAPSKWPA2PSK;OPEN");
            //                    mem.addValue("AuthMode", output);
            //                } else {
            //                    const char s[2] = ";";
            //                    char *token, output2[180];
            //                    int val = 0;
            //                    /* get the first token */
            //                    token = strtok(output, s);
            //                    while (token != NULL) {
            //                        token = strtok(NULL, s);
            //                        if (val)
            //                            strcat(output2, ";");
            //
            //                        if (val == 0)
            //                            strcpy(output2, token);
            //                        else if (val == 2)
            //                            strcat(output2, "OPEN");
            //                        else
            //                            strcat(output2, token);
            //
            //                        val++;
            //                    }
            //
            //                    if (val == 1)
            //                        strcat(output2, ";WPAPSKWPA2PSK;OPEN");
            //                    else if (val == 2)
            //                        strcat(output2, ";OPEN");
            //                    mem.addValue("AuthMode", output2);
            //                }
            //
            //                mem.getSetting("EncrypType", output);
            //                if (strchr(output, ';') == NULL) {
            //                    strcat(output, ";TKIPAES;OPEN");
            //                    mem.addValue("EncrypType", output);
            //                } else {
            //                    const char s[2] = ";";
            //                    char *token, output2[180];
            //                    int val = 0;
            //                    /* get the first token */
            //                    token = strtok(output, s);
            //                    while (token != NULL) {
            //                        token = strtok(NULL, s);
            //                        if (val)
            //                            strcat(output2, ";");
            //
            //                        if (val == 0)
            //                            strcpy(output2, token);
            //                        else if (val == 2)
            //                            strcat(output2, "NONE");
            //                        else
            //                            strcat(output2, token);
            //
            //                        val++;
            //                    }
            //
            //                    if (val == 1)
            //                        strcat(output2, ";TKIPAES;NONE");
            //                    else if (val == 2)
            //                        strcat(output2, ";NONE");
            //                    mem.addValue("EncrypType", output2);
            //                }
            //                mem.saveValues();
            //                //mem.commit();
            //                snprintf(buffer, 500, "{\"CommandType\":\"EnableSSID3\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\"}", mii);
            //                libwebsocket_write(wsi, (unsigned char *) buffer, strlen(buffer), LWS_WRITE_TEXT);
            //                system("reboot");
            //            } else {
            //                snprintf(buffer, 500, "{\"CommandType\":\"EnableSSID3\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"0\"}", mii);
            //            }
            //        }
            //            break;
            //        case IS_RANGE_EXTENDER_ENABLED:
            //        {
            //            int ret = isRangeExtenderEnabled();
            //            snprintf(buffer, 500, "{\"CommandType\":\"isRangeExtenderEnabled\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Reason\":\"0\",\"Output\":\"%d\"}", mii,ret);
            //        }
            //            break;
        case RULE_LIST:
        {
            char JSON[255000] = "";
            getRuleList_as_json1(JSON);
            snprintf(buffer, 256000, "{\"CommandType\":\"RuleList\" ,\"Rules\":{%s}", JSON);
        }
            break;
     //        case VARIABLE_LIST:
            //        {
            //            char JSON[2048] = "";
            //            int vfd = open("/tmp/rule_variables", O_RDWR);
            //            if(vfd == NULL){
            //                snprintf(buffer, 2048, "{\"CommandType\":\"VariableList\" ,\"Variables\":{%s}}", "cannot open: rule_variables");
            //            }
            //            else{
            //            	read(vfd, JSON, sizeof(JSON));
            //            	snprintf(buffer, 2048, "{\"CommandType\":\"VariableList\" ,\"Variables\":{%s}}", JSON);
            //            	close(vfd);
            //            }
            //        }
            //        break;
        case ADD_RULE:
        {
            char Rname[1024];
            int ruleID;
            char Triggers[10], results[10];
            if (getJsonValue("Name", Rname, arr, 1024)) {
                strcpy(Rname, decodestring(Rname));
                Rules newRule(Rname);
                ruleID = newRule.getID();
                char ruleid[10];
                sprintf(ruleid, "%d", ruleID);
                struct json_token *tt = NULL;
                if (getJsonValue("Triggers", Triggers, arr, 10) && getJsonValue("Results", results, arr, 10)) {
                    tt = find_json_token(arr, "Triggers");
                    sprintf(Triggers, "%d", (tt->num_desc / 15));
                    tt = find_json_token(arr, "Results");
                    sprintf(results, "%d", (tt->num_desc / 13));
                    if (gettriggerandresult_New(Triggers, ruleid, arr, 1)) {
                        if (gettriggerandresult_New(results, ruleid, arr, 2)) {
                            Event e;
                            e.event = RuleAdded;
                            RulesEvent re;
                            re.RuleId = newRule.getID();
                            memcpy(e.data, &re, sizeof (re));
                            HADevices::broadcastEvent(e);
                            snprintf(buffer, 500, "{\"CommandType\":\"AddRule\" ,\"Success\":\"true\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"1\"}", mii);

                        } else {
                            Rules ruleToDelete(ruleID);
                            ruleToDelete.removeRule();
                            snprintf(buffer, 500, "{\"CommandType\":\"AddRule\",\"Success\":\"false\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"1\"}", mii);
                        }
                    } else {
                        Rules ruleToDelete(ruleID);
                        ruleToDelete.removeRule();
                        snprintf(buffer, 500, "{\"CommandType\":\"AddRule\",\"Success\":\"false\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"2\"}", mii);
                    }
                   // Persist(1,"/tmp/rdb");
                } else {
                    Rules ruleToDelete(ruleID);
                    ruleToDelete.removeRule();
                    //Persist(1,"/tmp/rdb");
                    snprintf(buffer, 500, "{\"CommandType\":\"AddRule\",\"Success\":\"false\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"3\"}", mii);
                }
                if (tt) {
                    free(tt);
                    tt = NULL;
                }

            } else {
                snprintf(buffer, 500, "{\"CommandType\":\"AddRule\",\"Success\":\"false\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"4\"}", mii);
            }
        }
            break;
        case UPDATE_RULE:
        {
            char Rname[1024];
            int ruleID;
            char Triggers[10], results[10], ruleid[10];
            if (getJsonValue("ID", ruleid, arr, 10)) {
                ruleID = atoi(ruleid);
                try{
                    Rules r(ruleID);
                    r.removeAllTriggers();
                    r.removeAllResults();
                    getJsonValue("Name", Rname, arr, 1024);
                    strcpy(Rname, decodestring(Rname));
                    r.setName(Rname);
                    sprintf(ruleid, "%d", ruleID);
                    struct json_token *tt = NULL;
                    if (getJsonValue("Triggers", Triggers, arr, 10) && getJsonValue("Results", results, arr, 10)) {
                        tt = find_json_token(arr, "Triggers");
                        sprintf(Triggers, "%d", (tt->num_desc / 15));
                        tt = find_json_token(arr, "Results");
                        sprintf(results, "%d", (tt->num_desc / 13));
                        if (gettriggerandresult_New(Triggers, ruleid, arr, 1)) {
                            if (gettriggerandresult_New(results, ruleid, arr, 2)) {
                                snprintf(buffer, 500, "{\"CommandType\":\"UpdateRule\",\"Success\":\"true\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"1\"}", mii);
                                Event e;
                                e.event = RuleUpdated;
                                RulesEvent re;
                                re.RuleId = ruleID;
                                memcpy(e.data, &re, sizeof (re));
                                HADevices::broadcastEvent(e);

                            } else {
                                Rules ruleToDelete(ruleID);
                                ruleToDelete.removeRule();
                                snprintf(buffer, 500, "{\"CommandType\":\"UpdateRule\",\"Success\":\"false\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"1\"}", mii);
                            }
                        } else {
                            Rules ruleToDelete(ruleID);
                            ruleToDelete.removeRule();
                            snprintf(buffer, 500, "{\"CommandType\":\"UpdateRule\",\"Success\":\"false\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"1\"}", mii);
                        }


                    } else {
                        Rules ruleToDelete(ruleID);
                        ruleToDelete.removeRule();
                        snprintf(buffer, 500, "{\"CommandType\":\"UpdateRule\",\"Success\":\"false\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"1\"}", mii);
                    }
                    if (tt) {
                        free(tt);
                        tt = NULL;
                    }

                   // Persist(1,"/tmp/rdb");
                } catch (int ex) {
                    //failure   
                    snprintf(buffer, 500, "{\"CommandType\":\"UpdateRule\",\"Success\":\"false\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"1\"}", mii);
                }
            } else {
                snprintf(buffer, 500, "{\"CommandType\":\"UpdateRule\",\"Success\":\"false\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"1\"}", mii);
            }

        }
            break;
        case SET_RULE:
        {
            char ruleid[10], val[10];
            if ((getJsonValue("ID", ruleid, arr, 10)) && (getJsonValue("Value", val, arr, 10))) {
                int ruleID = 0;
                ruleID = atoi(ruleid);
                try{
                    Rules r(ruleID);
                    if (strstr(val, "true")) {
                        r.makeValid(true);
                        snprintf(buffer, 500, "{\"CommandType\":\"ValidateRule\",\"MobileInternalIndex\": \"%s\",\"ReasonCode\": \"0\",\"Success\": \"true\",\"Data\": {\"Value\": \"true\",\"ID\": \"%s\"}}", mii, ruleid);
                    } else if (strstr(val, "false")) {
                        snprintf(buffer, 500, "{\"CommandType\":\"ValidateRule\",\"MobileInternalIndex\": \"%s\",\"ReasonCode\": \"0\",\"Success\": \"true\",\"Data\": {\"Value\": \"false\",\"ID\": \"%s\"}}", mii, ruleid);
                        r.makeValid(false);
                    } else {
                        snprintf(buffer, 500, "{\"CommandType\":\"ValidateRule\",\"MobileInternalIndex\": \"%s\",\"ReasonCode\": \"0\",\"Success\": \"false\",\"Data\": {\"Value\": \"false\",\"ID\": \"%s\"}}", mii, ruleid);
                    }
                   // Persist(1,"/tmp/rdb");
                    Event e;
                    e.event = RuleUpdated;
                    RulesEvent re;
                    re.RuleId = r.getID();
                    memcpy(e.data, &re, sizeof (re));
                    HADevices::broadcastEvent(e);
                } catch (int i) {
                    snprintf(buffer, 500, "{\"CommandType\":\"ValidateRule\",\"MobileInternalIndex\": \"%s\",\"ReasonCode\": \"0\",\"Success\": \"false\",\"Data\": {\"Value\": \"true\",\"Ruleid\": \"%s\"}}", mii, ruleid);
                }
            } else {
                snprintf(buffer, 500, "{\"CommandType\":\"ValidateRule\",\"MobileInternalIndex\": \"%s\",\"ReasonCode\": \"0\",\"Success\": \"false\",\"Data\": {\"Value\": \"true\",\"Ruleid\": \"%s\"}}", mii, ruleid);
            }
        }
            break;
        case REMOVE_RULE_1:
        {
            char ruleid[10];
            if (getJsonValue("Rules.ID", ruleid, arr, 10)) {
                int ruleID;
                char Rname[1024];
                ruleID = atoi(ruleid);
                try{
                    Rules ruleToDelete(ruleID);
                    strcpy(Rname, specialchar(ruleToDelete.getName()));
                    if (ruleToDelete.removeRule()) {
                        snprintf(buffer, 500, "{\"CommandType\":\"RemoveRule\",\"Success\":\"true\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"0\"}", mii);
                        //Persist(1,"/tmp/rdb");
                        Event e;
                        e.event = RuleRemoved;
                        RulesEvent re;
                        re.RuleId = ruleID;
                        memcpy(e.data, &re, sizeof (re));
                        HADevices::broadcastEvent(e);
                    } else {
                        snprintf(buffer, 500, "{\"CommandType\":\"RemoveRule\",\"Success\":\"false\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"0\"}", mii);
                    }
                } catch (int i) {
                    snprintf(buffer, 500, "{\"CommandType\":\"RemoveRule\",\"Success\":\"false\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"0\"}", mii);
                }
            } else {
                snprintf(buffer, 500, "{\"CommandType\":\"RemoveRule\",\"Success\":\"false\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"0\"}", mii);
            }
        }
            break;
        case DELETE_ALL_RULES_1:
        {
            RuleList rl;
            rl.removeAllRules();
            snprintf(buffer, 500, "{\"CommandType\":\"RemoveAllRules\",\"Success\":\"true\",\"MobileInternalIndex\":\"%s\",\"ReasonCode\":\"0\"}", mii);
           // Persist(1,"/tmp/rdb");
            Event e;
            e.event = RuleRemoveAll;
            HADevices::broadcastEvent(e);
        }
            break;
        case CHECK_ASSOCIATION:
        {
            char IsSlave[10] = "";
            mem.getSetting("MeshIsSlave", IsSlave);
            if (!strcmp(IsSlave, "1")) {
                snprintf(buffer, 500, "{\"CommandType\":\"CheckAssociation\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Reason\":\"SlaveNode\"}", mii);

            } else {
                if (!alAssoc->checkAssocConnection()) {
                snprintf(buffer, 500, "{\"CommandType\":\"CheckAssociation\", \"MobileInternalIndex\":\"%s\",\"Success\":\"true\"}", mii);
            } else {
                snprintf(buffer, 500, "{\"CommandType\":\"CheckAssociation\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\"}", mii);
            }
            th_1 = true;
                alAssoc->closeAssocConnection();
            }
        }
            break;
        case ADD_DEVICE_1:
        {
            if (getJsonValue("Type", typeip, arr, 20)) {
                if (!strcmp(typeip, "HueBulb")) {
                    char id[20], username[50];
                    if (getJsonValue("ID", id, arr, 20)) {
                        if (getJsonValue("UserName", username, arr, 50)) {
                            if (!alAssoc->checkAssocConnection()) {
                                th_1 = false;
                                sprintf(AddMii, "%s", mii);
                                addwsi = wsi;
                                alAssoc->Hue_addDev(id, username);
                                searchcheck = 0;
                                // alarm(180);
                                //                    snprintf(buffer, 500, "{\"CommandType\":\"RemoveHueMac\", \"MobileInternalIndex\":\"%s\",\"Success\":\"true\"}", mii);
                            } else {
                                snprintf(buffer, 500, "{\"CommandType\":\"SearchHueMac\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Type\":\"HueHub\"}", mii);
                            }
                        }
                    }
                }
            } else {
            if (!alAssoc->checkAssocConnection()) {
                th_1 = false;
                sprintf(AddMii, "%s", mii);
                addwsi = wsi;
                //                alarm(120);
                alAssoc->userMeshID = GetMeshId();
                alAssoc->addDevZigbeeZwave();   // New
                char* buffer_local = (char*) malloc(100);
                snprintf(buffer_local, 100, "{\"CommandType\":\"AssocationInProgress\"}");
                libwebsocket_broadcast(&protocols[0], buffer_local);
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"AddDevice\",\"Success\":\"true\"}", mii);
                
//                char buff[1024];
//                sprintf(buff, (char *) "{\"CommandType\":\"1454\",\"MobileInternalIndex\":\"123\",\"CommandMode\":\"Request\"}");
//                SendToMeshServer(buff);
            } else {
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"AddDevice\",\"Success\":\"false\",\"Reason\":\"AlreadyInprogress\"}", mii);
            }

        }
        }
            break;
        case ASSOC_SET_NAME:
        {
            char Name[200] = "", Location[200] = "";
            if (!th_1) {
                if(getJsonValue("Location", Location, arr, 200)) {
                    alAssoc->setDevLocation(decodestring(Location));
                }
                if (getJsonValue("Name", Name, arr, 200)) {
                    alAssoc->setDevName(decodestring(Name));
                }
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"SetName\",\"Success\":\"true\"}", mii);
//                if (MeshID == 0) { //RAJA
                    alAssoc->addingDone();
//                }
                //RAJA
//                sprintf(buff, (char *) "{\"CommandType\":\"1472\",\"MobileInternalIndex\":\"123\",\"CommandMode\":\"Request\",\"MeshId\":\"%d\",\"LastAssocDevicetype\":\"%d\",\"Name\":\"%s\",\"Location\":\"%s\"}",MeshID,HAserver_Sending_Packet.Device_type,decodestring(Name),decodestring(Location));
//                LOG(Logger::INFO, buff);
//                SendToMeshServer(buff);
                    th_1 = true;
                alAssoc->closeAssocConnection();
            } else {
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"SetName\",\"Success\":\"false\"}", mii);
            }
        }
            break;
        case SELECT_DEVICETYPE:
        {
            char selected_device[100];
            if (!th_1) {
                if (getJsonValue("Type", selected_device, arr, 100)) {
                    if (strcmp(selected_device, "others") == 0) {
                        alAssoc->setDevType(baseDevType);
                    } else {
                        DevType dType = StrToDevType(selected_device);
                        alAssoc->setDevType(dType);
                    }
                }
                searchcheck = 0;
            } else {
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"SelectDevicetype\",\"Success\":\"false\"}", mii);
            }
        }
            break;
        case REMOVE_DEVICE:
        {
            if (!alAssoc->checkAssocConnection()) {
                th_1 = false;
                sprintf(AddMii, "%s", mii);
                addwsi = wsi;
                alAssoc->userMeshID = GetMeshId();
                alAssoc->removeDevZigbeeZwave();
                LOG(Logger::INFO, "Sending Remove command to zigbee_server and zwave_server");
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"RemoveDevice\",\"Success\":\"true\"}", mii);
                
                
//                char buff[1024];
//                sprintf(buff, (char *) "{\"CommandType\":\"1455\",\"MobileInternalIndex\":\"123\",\"CommandMode\":\"Request\"}");
//                SendToMeshServer(buff);
                //                alarm(60);
            } else {
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"RemoveDevice\",\"Success\":\"false\",\"Reason\":\"AssocationInProgress\"}", mii);
            }
        }
            break;
        case REMOVE_ALL_DEVICES:
        {
            if (!alAssoc->checkAssocConnection()) {
                th_1 = false;
                sprintf(AddMii, "%s", mii);
                addwsi = wsi;
                LOGINFO("Remove all devices");
                alAssoc->userMeshID = GetMeshId();
                alAssoc->doResetFor(reset_All);
//                resetZigbee();
//                char buff[1024];
//                sprintf(buff, (char *) "{\"CommandType\":\"1456\",\"MobileInternalIndex\":\"123\",\"CommandMode\":\"Request\"}");
//                LOG(Logger::INFO, buff);
//                SendToMeshServer(buff);
                th_1 = false;
                alAssoc->closeAssocConnection();
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"RemoveAllDevices\",\"Success\":\"true\"}", mii);
            } else {
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"RemoveAllDevices\",\"Success\":\"false\"}", mii);
            }
        }
            break;
        case CANCEL_PROCESS:
        {
            if ((!th_1) && (wsi == addwsi)) {
//                char buff[1024];
//                sprintf(buff, (char *) "{\"CommandType\":\"1457\",\"MobileInternalIndex\":\"123\",\"CommandMode\":\"Request\"}");
//                SendToMeshServer(buff);
                th_1 = true;
                alAssoc->sendCancelTo(cancel_ZigbeeZwave);
                alAssoc->closeAssocConnection();
                snprintf(buffer, 500, "{\"CommandType\":\"CancelProcess\", \"MobileInternalIndex\":\"%s\",\"Success\":\"true\"}", mii);
                LOGINFO("cancel function called 1457");
            } else {
                LOGINFO("NO cancel function called");
                snprintf(buffer, 500, "{\"CommandType\":\"CancelProcess\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\"}", mii);
            }
        }
            break;
        case DELETE_DEVICE:
        {
            char deviceid[10];
            int d_id, r;
            if (getJsonValue("DeviceId", deviceid, arr, 10)) {
                LOGINFO("delete device");
                d_id = atoi(deviceid);
                r = deletedevice(d_id);
                if (r) {
                    snprintf(buffer, 500, "{\"CommandType\":\"DeleteDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"true\"}", mii);
                } else {
                    snprintf(buffer, 500, "{\"CommandType\":\"DeleteDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\"}", mii);
                }
            } else {
                snprintf(buffer, 500, "{\"CommandType\":\"DeleteDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\"}", mii);
            }

        }
            break;
        case SEARCH_DEVICES:
        {
            if (getJsonValue("Type", typeip, arr, 20)) {
                if (!strcmp(typeip, "HueHub")) {
                    if (!alAssoc->checkAssocConnection()) {
                th_1 = false;
                        alAssoc->Hue_initiateAdd();
                sprintf(AddMii, "%s", mii);
                addwsi = wsi;
            } else {
                        snprintf(buffer, 500, "{\"CommandType\":\"SearchDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Type\":\"HueHub\"}", mii);
            }
                } else {
                    ;
                }
                searchcheck = 0;
            } else {
                snprintf(buffer, 500, "{\"CommandType\":\"SearchDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Type\":\"HueHub\"}", mii);

            }
            sprintf(cmdtype, "%s", "SearchDevice");
        }
            break;
        case ADD_IPDEVICES:
        {
            if (getJsonValue("Type", typeip, arr, 20)) {
                if (!strcmp(typeip, "HueHub")) {
            char id[20];
                    if (getJsonValue("ID", id, arr, 20)) {
                        if (!alAssoc->checkAssocConnection()) {
                    th_1 = false;
                    sprintf(AddMii, "%s", mii);
                    addwsi = wsi;
                            alAssoc->Hue_addBridge(id);
                            snprintf(buffer, 500, "{\"CommandType\":\"AddIPDevice\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Type\":\"HueHub\"}", AddMii);
                } else {
                            snprintf(buffer, 500, "{\"CommandType\":\"AddIPDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Type\":\"HueHub\"}", mii);
                }
            } else {

            }
                    sprintf(cmdtype, "%s", "AddIPDevice");
                } else if (!strcmp(typeip, "Nest")) {
                    char key[50] = "";
                    if (getJsonValue("Key", key, arr, 20)) {
                        if (!alAssoc->checkAssocConnection()) {
                            th_1 = false;
                            sprintf(AddMii, "%s", mii);
                            addwsi = wsi;
                            alAssoc->Nest_add(key);
                            searchcheck = 0; //                snprintf(buffer, 500, "{\"CommandType\":\"RemoveHue\", \"MobileInternalIndex\":\"%s\",\"Success\":\"true\"}", mii);
            } else {
                            snprintf(buffer, 500, "{\"CommandType\":\"AddIPDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Type\":\"Nest\"}", mii);
                        }
                    }
                } else {
                    snprintf(buffer, 500, "{\"CommandType\":\"AddIPDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Type\":\"Nest\"}", mii);
                }
            }
        }
            break;
        case REMOVE_IPDEVICES:
        {
            if (getJsonValue("Type", typeip, arr, 20)) {
                if (!strcmp(typeip, "HueHub")) {
            char id[20];
                    if (getJsonValue("ID", id, arr, 20)) {
                        if (!alAssoc->checkAssocConnection()) {
                    th_1 = false;
                    sprintf(AddMii, "%s", mii);
                    addwsi = wsi;
                            alAssoc->Hue_remove(id);
                   // searchcheck = 0;                    
                   snprintf(buffer, 500, "{\"CommandType\":\"RemoveIPDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Type\":\"HueHub\"}", mii);
                } else {
                            snprintf(buffer, 500, "{\"CommandType\":\"RemoveIPDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Type\":\"HueHub\"}", mii);
                }
            }
                } else if (!strcmp(typeip, "Nest")) {
                    if (!alAssoc->checkAssocConnection()) {
                        th_1 = false;
                        sprintf(AddMii, "%s", mii);
                        addwsi = wsi;
                        alAssoc->Nest_remove();
                         searchcheck = 0;
                    } else {
                        snprintf(buffer, 500, "{\"CommandType\":\"RemoveIPDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Type\":\"Nest\"}", mii);
                    }
                   
                            //                snprintf(buffer, 500, "{\"CommandType\":\"RemoveHue\", \"MobileInternalIndex\":\"%s\",\"Success\":\"true\"}", mii);
                } else {
                    snprintf(buffer, 500, "{\"CommandType\":\"RemoveIPDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Type\":\"Nest\"}", mii);
                }
            }
        }
            break;
        case REFRESH_DEVICES:
        {
            if (getJsonValue("Type", typeip, arr, 20)) {
                if (!strcmp(typeip, "HueHub")) {
                    char id[20], username[50];
                    if (getJsonValue("ID", id, arr, 20)) {
                        if (getJsonValue("UserName", username, arr, 50)) {
                            if (!alAssoc->checkAssocConnection()) {
                    th_1 = false;
                    sprintf(AddMii, "%s", mii);
                    addwsi = wsi;
                                alAssoc->Hue_refreshBridge(id, username);
                                snprintf(buffer, 500, "{\"CommandType\":\"RefreshDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"true\",\"Type\":\"HueHub\"}", AddMii);
 						searchcheck = 0;                   
 //                    snprintf(buffer, 500, "{\"CommandType\":\"RemoveHueMac\", \"MobileInternalIndex\":\"%s\",\"Success\":\"true\"}", mii);
                } else {
                                snprintf(buffer, 500, "{\"CommandType\":\"RefreshDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Type\":\"HueHub\"}", mii);
                }
                        } else {
                            snprintf(buffer, 500, "{\"CommandType\":\"RefreshDevice\", \"MobileInternalIndex\":\"%s\",\"Success\":\"false\",\"Type\":\"HueHub\"}", mii);
            }
                    }
                } else {
                    ;
                } 
            }
        }
            break;
        case CHECK_NEST_EXIST:
        {
            sprintf(AddMii, "%s", mii);
            addwsi = wsi;
            if (alAssoc->Nest_checkExist()) {
                snprintf(buffer, 500, "{\"CommandType\":\"CheckNestExist\",\"MobileInternalIndex\":\"%s\",\"Success\":\"true\"}", mii);
            } else {
            snprintf(buffer, 500, "{\"CommandType\":\"CheckNestExist\",\"MobileInternalIndex\":\"%s\",\"Success\":\"false\"}", mii);
            }

        }
            break;
        case DEVICELIST:
        {
            char JSON[255000] = "";
            getDeviceList_as_json_New(JSON);
            snprintf(buffer, 256000, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"DeviceList\",\"Devices\" : %s}", mii, JSON);
        }
            break;
               case INDEX_LIST:    
        {
            
            char JSON[255000] = "";
            getIndexList_as_json_New(JSON);
           
            snprintf(buffer, 512000, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"IndexList\",\"Index\" : %s}", mii, JSON);
            LOG(Logger::INFO, buffer);
        }
            break;
        case UPDATE_DEVICE_INDEX:
            {
            int r = 0;
            char dev[10] = "", idx[10] = "", val[50] = "";
            if (getJsonValue("ID", dev, arr, 10)) {
                if (getJsonValue("Index", idx, arr, 10)) {
                    if (getJsonValue("Value", val, arr, 50)) {
                    //                snprintf(buffer, 500, "{\"CommandType\":\"RemoveHue\", \"MobileInternalIndex\":\"%s\",\"Success\":\"true\"}", mii);
                        r = setValue(atoi(dev), atoi(idx), val);
                        //                    }
                    }
                }
            }
            if (r) { //failure
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"UpdateDeviceIndex\",\"Success\":\"false\"}", mii);
                } else {
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"UpdateDeviceIndex\",\"Success\":\"true\"}", mii);
                }
        }
            break;
        case GET_DEVICE_INDEX:
        {
            int r = 0;
            LOG(Logger::INFO, "GET_DEVICE_INDEX");
            char dev[10] = "", idx[10] = "", val[50] = "";
            if (getJsonValue("ID", dev, arr, 10)) {
                if (getJsonValue("Index", idx, arr, 10)) {
                    r = lua_getDeviceVal(atoi(dev), atoi(idx), val);
                }
            }
            if (r) { //failure
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"GetDeviceIndex\",\"Success\":\"false\"}", mii);
            } else { //success
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"GetDeviceIndex\",\"Success\":\"true\",\"Value\":\"%s\"}", mii, val);
            }
        }
            break;
        case UPDATE_DEVICE_NAME:
        {
            int r = 0;
            char dev[10] = "", Name[200] = "", Location[200] = "";
                    //                    snprintf(buffer, 500, "{\"CommandType\":\"RemoveHueMac\", \"MobileInternalIndex\":\"%s\",\"Success\":\"true\"}", mii);
            int s = getJsonValue("Location", Location, arr, 200);
            if (s) {
                strcpy(Location, decodestring(Location));
            } else {
                bzero(Location, strlen(Location));
            }
            if (getJsonValue("ID", dev, arr, 10)) {
                if (getJsonValue("Name", Name, arr, 200)) {
                    strcpy(Name, decodestring(Name));
                } else {
                    bzero(Name, strlen(Name));
                }
            }
            r = lua_setDevicePropertyAsync(atoi(dev), Name, Location);
            if (r) { //failure
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"UpdateDeviceName\",\"Success\":\"false\"}", mii);
            } else {
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"UpdateDeviceName\",\"Success\":\"true\"}", mii);
            }
        }
            break;
        case UPDATE_ALMOND_MODE:
        {
            FILE * fp;
            int r = 0;
            char Mode[10], Mailid[256];
            char Set_mode[300];
            if (getJsonValue("Mode", Mode, arr, 10)) {
                if (getJsonValue("EmailId", Mailid, arr, 256)) {
                    fp = fopen("/almond/email", "r");
                    if (fp != NULL) {

                        fgets(Mailid, 256, fp);
                        fclose(fp);
                    }
                    strcpy(Mailid, decodestring(Mailid));
                    snprintf(Set_mode, 300, "%s %s", Mode, Mailid);
                    LOG(Logger::INFO, Set_mode);
                    //r = al.setAlmondProDB(AlmondMode, Set_mode);
                    r = al.setAlmondModeToDisarm("LocalWebUI", Mailid, Mode[0]-'0');
                }
            }
            if (!r) { //failure
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"UpdateAlmondMode\",\"Success\":\"false\"}", mii);
            } else { //success
                snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"UpdateAlmondMode\",\"Success\":\"true\"}", mii);
            }
        }
            break;
        case 0:
        default:
        {

            snprintf(buffer, 500, "{\"MobileInternalIndex\":\"%s\",\"CommandType\":\"Unknown\"}", mii);
        }
            break;
    }
   

    if (searchcheck) {
        libwebsocket_write(wsi, (unsigned char *) buffer, strlen(buffer), LWS_WRITE_TEXT);
    } else {
        searchcheck = 1;
    }
}

void send_data(struct libwebsocket * wsi) {
    LOG(Logger::INFO, "Almond Mode state is sent");
    char data1[300],name[180];

    sl.Populate();
    cl.populate();
    al.getAlmondName(name);
    int i = 0, j = 0;
    char temp_name[100] = "";
    while (name[i] != '\0') {
        if (name[i] == '"' || name[i] == '\\') {
            temp_name[j] = '\\';
            j = j + 1;
            temp_name[j] = name[i];
        } else {
            temp_name[j] = name[i];
        }
        i++;
        j++;
    }
    temp_name[j] = '\0';
    snprintf(ALMONDNAME, 200, "%s", temp_name);
    al.getAlmondProDB(AlmondMode, data1);
    MODE_1 = data1[0] == '2' ? 2 : 3;
    snprintf(EMAIL, 256, "%s", data1 + 2);
	char *ptr=NULL;

	ptr = strchr(EMAIL, '\n');
               if (ptr != NULL) {
                   *ptr = '\0';
               }


    unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 500 + LWS_SEND_BUFFER_POST_PADDING];
    char *buffer = (char*) &buf[LWS_SEND_BUFFER_PRE_PADDING];

    snprintf(buffer, 500, "{\"CommandType\":\"DynamicAlmondModeUpdated\",\"Mode\":\"%d\",\"EmailId\":\"%s\"}", MODE_1, EMAIL);




//        LOG(Logger::INFO, "sdata1");
    libwebsocket_write(wsi, (unsigned char *) buffer, strlen(buffer), LWS_WRITE_TEXT);
    }

void theCallback(Device * p) {
    LOG(Logger::INFO, "received device callback");
    char dat[500];
    snprintf(dat, 500, "{\"CommandType\":\"DynamicIndexUpdated\",\"Devices\":{\"%d\":{\"DeviceValues\":{\"%d\":{\"Name\":\"%s\",\"Value\":\"%s\"}}}}}", p->getID(), p->getLastNotificationIndex(), p->getLastNotificationIndexName(), p->getLastNotificationIndexValue());
    libwebsocket_broadcast(&protocols[0], dat);
    //printf(" context->fds_count %d\n",

}

bool is_client_empty(Client c) {
    if (c.getClientID())
        return false;
    else
        return true;
}

bool is_scene_empty(Scene s) {
    if (s.getID())
        return false;
    else
        return true;
}

void theEventCallback(Event * e) {
    LOG(Logger::INFO, "Got Event Callback----------------------");
    char dat[20480];
    switch (e->event) {

        case DeviceAdded:
        {
            DeviceEvent dEvent;
            memcpy(&dEvent, e->data, sizeof (dEvent));
            Device d(dEvent.DevId);
            char JSON[2048];
            getLuaDeviceEntry_as_json_New(d, JSON);
            snprintf(dat, 4096, "{\"CommandType\":\"DynamicDeviceAdded\",\"Devices\":{\"%d\":%s}}", dEvent.DevId, JSON);
            libwebsocket_broadcast(&protocols[0], dat);
        }
            break;
        case DeviceRemoved:
        {
            DeviceEvent dEvent;
            memcpy(&dEvent, e->data, sizeof (dEvent));

            snprintf(dat, 500, "{\"CommandType\":\"DynamicDeviceRemoved\",\"Action\":\"remove\",\"Devices\":{\"%d\":{\"Type\":\"%d\"}}}", dEvent.DevId, dEvent.type);
            libwebsocket_broadcast(&protocols[0], dat);
            std::list<Scene>::iterator sit;
            std::list<SceneEntry>::iterator sei;
            for (sit = sl.Scenes.begin(); sit != sl.Scenes.end(); ++sit) {
                std::list<SceneEntry> se = sit->getSceneEntries();
                for (sei = se.begin(); sei != se.end(); ++sei) {
                    if (sei->DeviceID == dEvent.DevId) {
                        sit->Refresh();
						sei->Valid = false;
						//printf("sei->Valid %d\n",sei->Valid);
                       /// break;
                    }
                }
            }
           
        }
            break;
        case DeviceRemoveAll:
        {
            sl.Populate();
            snprintf(dat, 500, "{\"CommandType\":\"DynamicAllDevicesRemoved\"}");
            libwebsocket_broadcast(&protocols[0], dat);
        }
            break;

        case DeviceUpdated:
        {
            DeviceEvent dEvent;
            memcpy(&dEvent, e->data, sizeof (dEvent));
            Device d(dEvent.DevId);
            char JSON[2048];
            getLuaDeviceEntry_as_json_New(d, JSON);
            snprintf(dat, 4096, "{\"CommandType\":\"DynamicDeviceUpdated\",\"Devices\":{\"%d\":%s}}", dEvent.DevId, JSON);
            libwebsocket_broadcast(&protocols[0], dat);
        }
            break;
        case SceneAdded:
        {
            ScenesEvent sEvent;
            memcpy(&sEvent, e->data, sizeof (sEvent));
            Scene s(sEvent.SceneId);
            sl.Scenes.push_back(s);
            char localbuf[3584] = "";
            getLuaScenesEntry_as_json(s, localbuf);
            snprintf(dat, 4096, "{\"CommandType\":\"DynamicSceneAdded\",\"Scenes\":{%s}}", localbuf);
            libwebsocket_broadcast(&protocols[0], dat);
        }
            break;
        case SceneRemoved:
        {
            ScenesEvent sEvent;
            memcpy(&sEvent, e->data, sizeof (sEvent));
            //snprintf(dat, 500, "{\"CommandType\":\"DynamicSceneRemoved\",\"Scenes\":{\"ID\":%d}}", sEvent.SceneId);
	    snprintf(dat, 500, "{\"CommandType\":\"DynamicSceneRemoved\",\"Action\": \"remove\",\"Scenes\":{\"%d\":{}}}", sEvent.SceneId);
            libwebsocket_broadcast(&protocols[0], dat);
            std::list<Scene>::iterator it;
            for (it = sl.Scenes.begin(); it != sl.Scenes.end(); ++it) {
                if ((it->getID() == sEvent.SceneId)) {
                    it = sl.Scenes.erase(it);
                    break;
                }
            }
        }
            break;
        case SceneUpdated:
        {
            ScenesEvent sEvent;
            memcpy(&sEvent, e->data, sizeof (sEvent));

            std::list<Scene>::iterator it;
            for (it = sl.Scenes.begin(); it != sl.Scenes.end(); ++it) {
                if ((it->getID() == sEvent.SceneId)) {
                    it->Refresh();
                    char localbuf[3584] = "";
                    getLuaScenesEntry_as_json(*it, localbuf);
                    snprintf(dat, 4096, "{\"CommandType\":\"DynamicSceneUpdated\",\"Scenes\":{%s}}", localbuf);
                    libwebsocket_broadcast(&protocols[0], dat);
                    break;
                }
            }
        }
            break;
        case SceneRemoveAll:
        {
            // memcpy(&sevent, e->data, sizeof (sevent));
            sl.Scenes.clear();
            snprintf(dat, 500, "{\"CommandType\":\"DynamicAllScenesRemoved\"}");
            libwebsocket_broadcast(&protocols[0], dat);
        }
            break;
        case SceneActivated:
        {
            ScenesEvent sEvent;
            memcpy(&sEvent, e->data, sizeof (sEvent));

            std::list<Scene>::iterator it;
            for (it = sl.Scenes.begin(); it != sl.Scenes.end(); ++it) {
                if ((it->getID() == sEvent.SceneId)) {
                    it->Refresh();
                    snprintf(dat, 4096, "{\"CommandType\":\"DynamicSceneActivated\",\"Action\":\"update\",\"Scenes\":{\"%d\":{\"Active\":\"true\",\"LastActiveEpoch\":\"%llu\"}}}", it->getID(),it->LastActivatedTime);
                    libwebsocket_broadcast(&protocols[0], dat);
                    break;
                }
            }
        }
            break;
        case SceneDeactivated:
        {
            ScenesEvent sEvent;
            memcpy(&sEvent, e->data, sizeof (sEvent));

            std::list<Scene>::iterator it;
            for (it = sl.Scenes.begin(); it != sl.Scenes.end(); ++it) {
                if ((it->getID() == sEvent.SceneId)) {
                    it->Refresh();
                    snprintf(dat, 4096, "{\"CommandType\":\"DynamicSceneActivated\",\"Action\":\"update\",\"Scenes\":{\"%d\":{\"Active\":\"false\",\"LastActiveEpoch\":\"%llu\"}}}", it->getID(),it->LastActivatedTime);
                    libwebsocket_broadcast(&protocols[0], dat);
                    break;
                }
            }

        }
            break;
        case RuleAdded:
        {
            RulesEvent revent;
            char buf[20480];
            memcpy(&revent, e->data, sizeof (revent));
            Rules r(revent.RuleId);
            getLuaRuleEntry_as_json2(r, buf);
            snprintf(dat, 20480, "{\"CommandType\":\"DynamicRuleAdded\",\"Rules\":{%s}}", buf);
            libwebsocket_broadcast(&protocols[0], dat);
        }
            break;
        case RuleRemoved:
        {
            RulesEvent revent;
            memcpy(&revent, e->data, sizeof (revent));
            //snprintf(dat, 500, "{\"CommandType\":\"DynamicRuleRemoved\",\"Rules\":{\"ID\":\"%d\"}}", revent.RuleId);
	    snprintf(dat, 500, "{\"CommandType\":\"DynamicRuleRemoved\",\"Action\": \"Remove\",\"Rules\":{\"%d\":{}}}", revent.RuleId);
	    libwebsocket_broadcast(&protocols[0], dat);

        }
            break;
        case RuleUpdated:
        {
            RulesEvent revent;
            char buf[20480];
            memcpy(&revent, e->data, sizeof (revent));
            Rules r(revent.RuleId);
            getLuaRuleEntry_as_json2(r, buf);
            snprintf(dat, 20480, "{\"CommandType\":\"DynamicRuleUpdated\",\"Rules\":{%s}}", buf);
            libwebsocket_broadcast(&protocols[0], dat);
        }
            break;
        case RuleRemoveAll:
        {
            RulesEvent revent;
            memcpy(&revent, e->data, sizeof (revent));
            snprintf(dat, 500, "{\"CommandType\":\"DynamicAllRulesRemoved\"}");
            libwebsocket_broadcast(&protocols[0], dat);
        }
            break;
        case AlmondModeUpdated:
        {
            if ((strstr((char *) e->data, "Name="))) {
                snprintf(t_temp, 1024, "AlmondModeUpdated %s", e->data);
                LOG(Logger::INFO, t_temp);
                int i = 0, j = 0;
                char temp_name[100] = "";
                while (e->data[i] != '\0') {
                    if (e->data[i] == '"' || e->data[i] == '\\') {
                        temp_name[j] = '\\';
                        j = j + 1;
                        temp_name[j] = e->data[i];
                    } else {
                        temp_name[j] = e->data[i];
                    }
                    i++;
                    j++;
                }
                temp_name[j] = '\0';
                snprintf(ALMONDNAME, 200, "%s", temp_name + 5);
                snprintf(dat, 500, "{\"CommandType\":\"DynamicAlmondNameUpdated\",\"Name\":\"%s\"}", ALMONDNAME);
                LOG(Logger::INFO, dat);
                libwebsocket_broadcast(&protocols[0], dat);
            } else {
                snprintf(t_temp, 1024, "AlmondModeUpdated %s", e->data);
                LOG(Logger::INFO, t_temp);
                MODE_1 = e->data[5] == '2' ? 2 : 3;
                snprintf(EMAIL, 256, "%s", e->data + 7);
		char *ptr=NULL;

		ptr = strchr(EMAIL, '\n');
               if (ptr != NULL) {
                   *ptr = '\0';
               }
                snprintf(dat, 500, "{\"CommandType\":\"DynamicAlmondModeUpdated\",\"Mode\":\"%c\",\"EmailId\":\"%s\"}", e->data[5], e->data + 7);
                LOG(Logger::INFO, dat);
                libwebsocket_broadcast(&protocols[0], dat);
            }
            break;
        }
        case AlmondUpdated:
        {
            char buf[100] = "";
            RouterData rDat;
            memcpy(&rDat, e->data, sizeof (rDat));

            switch (rDat.DataType) {
                case AlmondMode:
                {
                    sprintf(buf, "Received Mode %s Data", rDat.Data);
                    LOG(Logger::PRINT, buf);
                    snprintf(dat, 500, "{\"CommandType\":\"DynamicAlmondModeUpdated\",\"Mode\":\"%c\",\"EmailId\":\"%s\"}", rDat.Data[0], rDat.Data + 1);
                    LOG(Logger::INFO, dat);
                    libwebsocket_broadcast(&protocols[0], dat);
                }
                    break;
                case AlmondName:
                {
                    sprintf(buf, "Received Mode %s Data", rDat.Data);
                    LOG(Logger::PRINT, buf);
                    int i = 0, j = 0;
                    char temp_name[100] = "";
                    while (rDat.Data[i] != '\0') {
                        if (rDat.Data[i] == '"' || rDat.Data[i] == '\\') {
                            temp_name[j] = '\\';
                            j = j + 1;
                            temp_name[j] = rDat.Data[i];
                        } else {
                            temp_name[j] = rDat.Data[i];
                        }
                        i++;
                        j++;
                    }
                    temp_name[j] = '\0';
                    snprintf(ALMONDNAME, 200, "%s", temp_name);
                    snprintf(dat, 500, "{\"CommandType\":\"DynamicAlmondNameUpdated\",\"Name\":\"%s\"}", ALMONDNAME);
                    LOG(Logger::INFO, dat);
                    libwebsocket_broadcast(&protocols[0], dat);
                }
                    break;
            }

        }
            break;
        case ClientJoined:
        {
            ClientEvent cEvent;
            memcpy(&cEvent, e->data, sizeof (cEvent));
            std::list<Client>::iterator it;
            for (it = cl.clientsList.begin(); it != cl.clientsList.end(); ++it) {
                if (strcmp(it->getMac(), cEvent.mac) == 0) {
                    it->refresh();
                  //  if (!WIRELESSMODE) {
                        char clientbuf[1024] = "";
                        getLuaClientsEntry_as_json(*it, clientbuf);
                        snprintf(dat, 1024, "{\"CommandType\":\"DynamicClientJoined\",\"Clients\":{%s}}", clientbuf);
                        libwebsocket_broadcast(&protocols[0], dat);
                  //  }
                    break;
                }
            }
            if (it == cl.clientsList.end()) {
                Client ct(cEvent.ClientID);
                cl.clientsList.push_back(ct);
              //  if (!WIRELESSMODE) {
                    char clientbuf[1024] = "";
                    getLuaClientsEntry_as_json(ct, clientbuf);
                    snprintf(dat, 1024, "{\"CommandType\":\"DynamicClientAdded\",\"Clients\":{%s}}", clientbuf);
                    libwebsocket_broadcast(&protocols[0], dat);
              //  }

            }
        }
            break;
        case ClientLeft:
        {
            ClientEvent cEvent;
            memcpy(&cEvent, e->data, sizeof (cEvent));
            std::list<Client>::iterator it;
            for (it = cl.clientsList.begin(); it != cl.clientsList.end(); ++it) {
                if (strcmp(it->getMac(), cEvent.mac) == 0) {
                    it->refresh();
                    break;
                }
            }
          //  if (!WIRELESSMODE) {
                char clientbuf[1024] = "";
                getLuaClientsEntry_as_json(*it, clientbuf);
                snprintf(dat, 1024, "{\"CommandType\":\"DynamicClientLeft\",\"Clients\":{%s}}", clientbuf);
                libwebsocket_broadcast(&protocols[0], dat);
          //  }
        }
            break;
        case ClientUpdated:
        {

            ClientEvent cevent;
            LOG(Logger::PRINT, "Received ClientUpdated event");
            memcpy(&cevent, e->data, sizeof (cevent));
            std::list<Client>::iterator it;
            for (it = cl.clientsList.begin(); it != cl.clientsList.end(); ++it) {
                if (strcmp(it->getMac(), cevent.mac) == 0) {
                    it->refresh();
                    break;
                }
            }
          //  if (!WIRELESSMODE) {
                char clientbuf[1024] = "";
                getLuaClientsEntry_as_json(*it, clientbuf);
                snprintf(dat, 4096, "{\"CommandType\":\"DynamicClientUpdated\",\"Clients\":{%s}}", clientbuf);
                libwebsocket_broadcast(&protocols[0], dat);
          //  }
        }
            break;
        case ClientRemoved:
        {
            ClientEvent cEvent;
            memcpy(&cEvent, e->data, sizeof (cEvent));
            LOG(Logger::PRINT, cEvent.mac);
            LOG(Logger::PRINT, "Received ClientRemoved event");
           // if (!WIRELESSMODE) {
		snprintf(dat, 500, "{\"CommandType\":\"DynamicClientRemoved\", \"Clients\":{\"%d\":{\"MAC\":\"%s\"}}}", cEvent.ClientID, cEvent.mac);
//                snprintf(dat, 500, "{\"CommandType\":\"DynamicClientRemoved\", \"Clients\":{\"%d\":{\"MAC\":\"%s\"}}}", cEvent.ClientID, cEvent.mac);
                libwebsocket_broadcast(&protocols[0], dat);
          //  }
            std::list<Client>::iterator it;
            for (it = cl.clientsList.begin(); it != cl.clientsList.end(); ++it) {
                if (strcmp(it->getMac(), cEvent.mac) == 0) {
                    it = cl.clientsList.erase(it);
                    break;
                }
            }
            for (it = cl.clientsList.begin(); it != cl.clientsList.end(); ++it) {
                LOG(Logger::PRINT, it->getMac());
            }
        }
            break;
        case ClientRemoveAll:
        {
            cl.clientsList.clear();
         //   if (!WIRELESSMODE) {
                snprintf(dat, 500, "{\"CommandType\":\"DynamicAllClientsRemoved\"}");
                libwebsocket_broadcast(&protocols[0], dat);
         //   }
        }
            break;
        case MeshSlaveRemoveAll:
        {
            LOG(Logger::PRINT, "Received MeshSlaveRemoveAll event");
            ml.meshSlaveList.clear();
        }
            break;
        case Unknown:
        default:
        {

            LOG(Logger::PRINT, "Unknown event received");
        }


    }
}

int main(int argc, char **argv) {
	int n = 0;
	int port = 7681;
	int use_ssl = 0;
	struct libwebsocket_context *context;
	int opts = 0;
	char interface_name[128] = "";
	const char *interface = NULL;
	char ssl_cert[256] = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
	char ssl_key[256] = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
#ifndef WIN32
	int syslog_options = LOG_PID | LOG_PERROR;
#endif
	int client = 0;
	int listen_port = 80;
	struct lws_context_creation_info info;
	char passphrase[256];
	char uri[256] = "/";
#ifndef LWS_NO_CLIENT
    char address[256];
    //	int rate_us = 250000;
    int rate_us = 250000;
	int disallow_selfsigned = 0;
#endif

	int debug_level = 0;
#ifndef LWS_NO_DAEMONIZE
//    int daemonize = 0;
    int daemonize = 0;
#endif

	memset(&info, 0, sizeof info);

#ifndef LWS_NO_CLIENT
	lwsl_notice("Built to support client operations\n");
#endif
#ifndef LWS_NO_SERVER
	lwsl_notice("Built to support server operations\n");
#endif

	while (n >= 0) {
		n = getopt_long(argc, argv, "i:hsp:d:DC:k:P:vu:"
#ifndef LWS_NO_CLIENT
			"c:r:"
#endif
				, options, NULL);
		if (n < 0)
			continue;
		switch (n) {
		case 'P':
			strncpy(passphrase, optarg, sizeof(passphrase));
			passphrase[sizeof(passphrase) - 1] = '\0';
			info.ssl_private_key_password = passphrase;
			break;
		case 'C':
			strncpy(ssl_cert, optarg, sizeof(ssl_cert));
			ssl_cert[sizeof(ssl_cert) - 1] = '\0';
			disallow_selfsigned = 1;
			break;
		case 'k':
			strncpy(ssl_key, optarg, sizeof(ssl_key));
			ssl_key[sizeof(ssl_key) - 1] = '\0';
			break;
		case 'u':
			strncpy(uri, optarg, sizeof(uri));
			uri[sizeof(uri) - 1] = '\0';
			break;
			
#ifndef LWS_NO_DAEMONIZE
		case 'D':
//                daemonize = 1;
                daemonize = 1;
#ifndef WIN32
			syslog_options &= ~LOG_PERROR;
#endif
			break;
#endif
#ifndef LWS_NO_CLIENT
		case 'c':
			client = 1;
			strncpy(address, optarg, sizeof(address) - 1);
			address[sizeof(address) - 1] = '\0';
			port = 80;
			break;
		case 'r':
                //			rate_us = atoi(optarg) * 1000;
                rate_us = atoi(optarg) * 1000;
			break;
#endif
		case 'd':
			debug_level = atoi(optarg);
			break;
		case 's':
			use_ssl = 1; /* 1 = take care about cert verification, 2 = allow anything */
			break;
		case 'p':
			port = atoi(optarg);
			break;
		case 'v':
			versa = 1;
			break;
		case 'i':
			strncpy(interface_name, optarg, sizeof interface_name);
			interface_name[(sizeof interface_name) - 1] = '\0';
			interface = interface_name;
			break;
		case '?':
		case 'h':
			fprintf(stderr, "Usage: libwebsockets-test-echo\n"
				"  --debug      / -d <debug bitfield>\n"
				"  --port       / -p <port>\n"
				"  --ssl-cert   / -C <cert path>\n"
				"  --ssl-key    / -k <key path>\n"
#ifndef LWS_NO_CLIENT
				"  --client     / -c <server IP>\n"
				"  --ratems     / -r <rate in ms>\n"
#endif
				"  --ssl        / -s\n"
				"  --passphrase / -P <passphrase>\n"
				"  --interface  / -i <interface>\n"
#ifndef LWS_NO_DAEMONIZE
				"  --daemonize  / -D\n"
#endif
			);
			exit(1);
		}
	}

#ifndef LWS_NO_DAEMONIZE 
	/*
	 * normally lock path would be /var/lock/lwsts or similar, to
	 * simplify getting started without having to take care about
	 * permissions or running as root, set to /tmp/.lwsts-lock
	 */
#if defined(WIN32) || defined(_WIN32)
#else
//    if (!client && daemonize && lws_daemonize("/tmp/.lwstecho-lock")) {
//        fprintf(stderr, "Failed to daemonize\n");
//        return 1;
//    }
    if (!client && daemonize && lws_daemonize("/tmp/.lwstecho-lock")) {
        fprintf(stderr, "Failed to daemonize\n");
        return 1;
    }
#endif
#endif

#ifdef WIN32
#else
	/* we will only try to log things according to our debug_level */
	setlogmask(LOG_UPTO (LOG_DEBUG));
	openlog("lwsts", syslog_options, LOG_DAEMON);

	/* tell the library what debug level to emit and to send it to syslog */
	lws_set_log_level(debug_level, lwsl_emit_syslog);
#endif
	lwsl_notice("libwebsockets echo test - "
		    "(C) Copyright 2010-2015 Andy Green <andy@warmcat.com> - "
		    "licensed under LGPL2.1\n");
#ifndef LWS_NO_CLIENT
	if (client) {
		lwsl_notice("Running in client mode\n");
		listen_port = CONTEXT_PORT_NO_LISTEN;
		if (use_ssl && !disallow_selfsigned) {
			lwsl_info("allowing selfsigned\n");
			use_ssl = 2;
		} else {
			lwsl_info("requiring server cert validation againts %s\n", ssl_cert);
			info.ssl_ca_filepath = ssl_cert;
		}
	} else {
#endif
#ifndef LWS_NO_SERVER
		lwsl_notice("Running in server mode\n");
		listen_port = port;
#endif
#ifndef LWS_NO_CLIENT
	}
#endif

	info.port = listen_port;
	info.iface = interface;
	info.protocols = protocols;
#ifndef LWS_NO_EXTENSIONS
	info.extensions = libwebsocket_get_internal_extensions();
#endif
	if (use_ssl && !client) {
		info.ssl_cert_filepath = ssl_cert;
		info.ssl_private_key_filepath = ssl_key;
	} else
		if (use_ssl && client) {
			info.ssl_cert_filepath = NULL;
			info.ssl_private_key_filepath = NULL;
		}
	info.gid = -1;
	info.uid = -1;
	info.options = opts;

	context = libwebsocket_create_context(&info);
	if (context == NULL) {
		lwsl_err("libwebsocket init failed\n");
		return -1;
	}


	//signal(SIGINT, sighandler);

	n = 0;
    HADevices::webInitialize();
    HADevices::genericCallback(theCallback);
    HADevices::eventCallback(&theEventCallback);
    libAssocInit_web();
    logger = Logger::getInstance();
    char logfile[100] = "/tmp/webServer.log";
    logger->setFileName(logfile);
    logger->setMethod(Logger::ALL, Logger::LOGFILE);

    LOG(Logger::INFO, "=================== webserver started @RAJA===================");

    cl.populate();
    sl.Populate();
	while (n >= 0) { // && !force_exit) {
        n = libwebsocket_service(context, 50);
		}

	libwebsocket_context_destroy(context);


	return 0;
}

void getRuleList_as_json1(char *JSON) {

    RuleList rl;
    rl.populate();
    char buf[20480], localbuf[100];
    list<Rules>::iterator it;
    int i;

    for (i = 0, it = rl.ruleList.begin(); it != rl.ruleList.end(); ++it, ++i) {
        getLuaRuleEntry_as_json2(*it, buf);
        if (i > 0) {
            bzero(localbuf, strlen(localbuf));
            sprintf(localbuf, ",");
            strcat(JSON, localbuf);
        }

        strcat(JSON, buf);

    }
    sprintf(localbuf, "}");
    strcat(JSON, localbuf);

}

void getLuaRuleEntry_as_json2(Rules &r, char *buf) {
    char Rname[1024] = "", TriBuff[512] = "", ResBuff[512] = "", localbuf[100] = "";
    strcpy(Rname, specialchar(r.getName()));
    snprintf(buf, 512, "\"%d\":{\"ID\":\"%d\",\"Name\":\"%s\",\"Valid\":\"%s\",\"Triggers\":[", r.getID(),r.getID(), Rname,(r.isValid() ? "true" : "false") );
    list<RuleTrigger> rT = r.getRuleTriggers();
    int idx_cnt = rT.size();
    list<RuleTrigger>::iterator it = rT.begin();
    for (int i = 0; i < idx_cnt || it != rT.end(); i++, ++it) {
        if (i > 0) {
            sprintf(localbuf, ",");
            strcat(buf, localbuf);
        }

        if ((*it).TriggerType == DeviceTriggers) {
            char arr[5] = "";
            enumToConditionString((*it).ConditionType, arr);
            snprintf(TriBuff, 512, "{ \"Type\":\"DeviceTrigger\",\
            \"ID\":\"%d\",\"Index\":\"%d\",\"Grouping\":\"%s\",\
            \"Valid\":\"%s\",\"Condition\":\"%s\",\"Value\":\"%s\"}", (*it).DeviceID, (*it).DeviceIndex,
                    ((*it).TriggerGrouping ? "OR" : "AND"), ((*it).Valid ? "true" : "false"), arr, (*it).IndexValue);
        } else if ((*it).TriggerType == EventTriggers) {
            char arr[5] = "";
            char etype[36] = "";
            enumToConditionString((*it).ConditionType, arr);
            getEventAsString((*it).eventType, etype);
            snprintf(TriBuff, 512, "{\"Type\":\"EventTrigger\",\
            \"EventType\":\"%s\",\"ID\":\"%d\",\"Grouping\":\"%s\",\"Condition\":\"%s\",\
            \"Valid\":\"%s\",\"Value\":\"%s\"}", etype, (*it).eventId,
                    ((*it).TriggerGrouping ? "OR" : "AND"), arr, ((*it).Valid ? "true" : "false"), (*it).eventValue);
        } else if ((*it).TriggerType == TimeTriggers) {
            snprintf(TriBuff, 512, "{\"Type\":\"TimeTrigger\",\
            \"Range\":\"%d\",\"Hour\":\"%d\",\"Minutes\":\"%d\",\"DayOfMonth\":\"%s\",\"MonthOfYear\":\"%s\",\"DayOfWeek\":\"%s\",\
            \"Grouping\":\"%s\",\"Valid\":\"%s\"}", (*it).timeTrigger.range,
                    (*it).timeTrigger.hours, (*it).timeTrigger.mins, (*it).timeTrigger.DayOfMonth,
                    (*it).timeTrigger.MonthOfYear, (*it).timeTrigger.DayOfWeek, ((*it).TriggerGrouping ? "OR" : "AND"), ((*it).Valid ? "true" : "false"));
        } else if ((*it).TriggerType == WeatherTriggers) {
            char arr[5] = "";
            char etype[36] = "";
            enumToConditionString((*it).ConditionType, arr);
            getWeatherTypeAsString((*it).weatherType, etype);

            snprintf(TriBuff, 512, "{\"Type\":\"WeatherTrigger\",\
            \"WeatherType\":\"%s\",\"Duration\":\"%lu\",\"Grouping\":\"%s\",\"Condition\":\"%s\",\"Valid\":\"%s\",\
            \"Value\":\"%s\"}", etype,
                    (*it).Duration, ((*it).TriggerGrouping ? "OR" : "AND"), arr,
                    ((*it).Valid ? "true" : "false"), (*it).weatherValue);

        } else if ((*it).TriggerType == VariableTriggers) {
            char arr[5] = "";
            enumToConditionString((*it).ConditionType, arr);
            if((*it).eventId == 1 || (*it).eventId == 2){
                snprintf(TriBuff, 512, "{ \"Type\":\"VariableTrigger\",\"Grouping\":\"%s\",\
            \"Valid\":\"%s\",\"Condition\":\"%s\",\"Name\":\"%s\",\"Value\":\"%s\"}", ((*it).TriggerGrouping ? "OR" : "AND"), ((*it).Valid ? "true" : "false"), arr, (*it).IndexValue, (*it).eventValue);
            }else{
                snprintf(TriBuff, 512, "{ \"Type\":\"VariableTrigger\",\"Grouping\":\"%s\",\
            \"Valid\":\"%s\",\"Condition\":\"%s\",\"Name\":\"%s\",\"Value\":\"%d\"}", ((*it).TriggerGrouping ? "OR" : "AND"), ((*it).Valid ? "true" : "false"), arr, (*it).IndexValue, (*it).DeviceIndex);
            }
            
        }
        strcat(buf, TriBuff);
    }
    sprintf(localbuf, "],\"Results\":[");
    strcat(buf, localbuf);

    std::list<RuleResult> rR = r.getRuleResults();
    idx_cnt = rR.size();
    list<RuleResult>::iterator rrI = rR.begin();
    for (int i = 0; i < idx_cnt || rrI != rR.end(); i++, ++rrI) {
        if (i > 0) {
            sprintf(localbuf, ",");
            strcat(buf, localbuf);
        }

        if ((*rrI).type == DeviceResult) {
            snprintf(ResBuff, 512, "{\"Type\":\"DeviceResult\",\
            \"ID\":\"%d\",\"Index\":\"%d\",\"PreDelay\":\"%d\",\"Valid\":\"%s\",\"Value\":\"%s\"}",
                    (*rrI).DeviceID, (*rrI).DeviceIndex, (*rrI).PreDelay,
                    ((*rrI).Valid ? "true" : "false"), (*rrI).IndexValue);
        } else if ((*rrI).type == EventResult) {
            char etype[36] = "";
            getEventAsString((*rrI).eventType, etype);
            snprintf(ResBuff, 512, "{\"Type\":\"EventResult\",\"EventType\":\"%s\",\
            \"ID\":\"%d\",\"PreDelay\":\"%d\",\"Valid\":\"%s\",\"Value\":\"%s\"}",
                    etype, (*rrI).eventId, (*rrI).PreDelay, ((*rrI).Valid ? "true" : "false"), (*rrI).eventValue);
        } else if ((*rrI).type == SceneResult) {
            snprintf(ResBuff, 512, "{\"Type\":\"SceneResult\",\"ID\":\"%d\",\
            \"PreDelay\":\"%d\",\"Valid\":\"%s\"}", (*rrI).SceneID, (*rrI).PreDelay, ((*rrI).Valid ? "true" : "false"));

        } else if ((*rrI).type == NetworkResult) {
            snprintf(ResBuff, 512, "{\"Type\":\"NetworkResult\",\"ID\":\"%d\",\
            \"Value\":\"%s\",\"PreDelay\":\"%d\",\"Valid\":\"%s\"}", (*rrI).eventId, (*rrI).eventValue, (*rrI).PreDelay, ((*rrI).Valid ? "true" : "false"));

        } else if ((*rrI).type == MessageResult) {
            char mtype[36] = "";
            getWeatherTypeAsString((*rrI).weatherType, mtype);
            snprintf(ResBuff, 512, "{\"Type\":\"MessageResult\",\"MessageType\":\"%s\",\"ID\":\"%d\",\"Value\":\"%s\",\"PreDelay\":\"%d\",\"Valid\":\"%s\"}", mtype, (*rrI).MsgID, (*rrI).msgValue, (*rrI).PreDelay, ((*rrI).Valid ? "true" : "false"));
        } else if ((*rrI).type == VariableResult) {
            if((*rrI).MsgID == 1){
            snprintf(ResBuff, 512, "{\"Type\":\"VariableResult\",\
            \"PreDelay\":\"%d\",\"Valid\":\"%s\",\"Name\":\"%s\",\"Value\":\"%c%c%d\"}",
                    (*rrI).PreDelay, ((*rrI).Valid ? "true" : "false"), (*rrI).IndexValue, (*rrI).msgValue[0], (*rrI).eventValue[0], (*rrI).DeviceIndex);
            }else if((*rrI).MsgID == 2){
            snprintf(ResBuff, 512, "{\"Type\":\"VariableResult\",\
            \"PreDelay\":\"%d\",\"Valid\":\"%s\",\"Name\":\"%s\",\"Value\":\"%c%c%c\"}",
                    (*rrI).PreDelay, ((*rrI).Valid ? "true" : "false"), (*rrI).IndexValue, (*rrI).msgValue[0], (*rrI).eventValue[0], (*rrI).weathreValue[0]);
            }else{
                snprintf(ResBuff, 512, "{\"Type\":\"VariableResult\",\
            \"PreDelay\":\"%d\",\"Valid\":\"%s\",\"Name\":\"%s\",\"Value\":\"%d\"}",
                    (*rrI).PreDelay, ((*rrI).Valid ? "true" : "false"), (*rrI).IndexValue, (*rrI).DeviceIndex);
            }
        }
        strcat(buf, ResBuff);

    }
    sprintf(localbuf, "]}");
    strcat(buf, localbuf);

}

bool AddTrigger_New(char *ruleid, struct json_token *tt) {

    char triggertype[100] = "", temp[50] = "", temp1[50] = "", temp2[50] = "", temp3[50] = "", temp4[50] = "", temp5[50] = "";
    
    //int k = (curr->num_desc / 15);
    //    struct json_token *curr;
    struct json_token *curr;
    bool checkupdate = false;
    //    curr = 
    curr = find_json_token(tt, "Triggers");
   
    for (int i = 0; i < 100; i++) {
        sprintf(temp, "Triggers[%d].Type", i);
        if (getJsonValue(temp, triggertype, tt, 100)) {
            if (!strcmp(triggertype, "DeviceTrigger")) {
                sprintf(temp, "Triggers[%d].Condition", i);
                sprintf(temp1, "Triggers[%d].ID", i);
                sprintf(temp2, "Triggers[%d].Index", i);
                sprintf(temp3, "Triggers[%d].Value", i);
                sprintf(temp4, "Triggers[%d].Grouping", i);
                char devid[10], devindex[10], indexval[20], group[10], condition[10], duration[20];
                if ((getJsonValue(temp1, devid, tt, 10)) && (getJsonValue(temp2, devindex, tt, 10)) && (getJsonValue(temp3, indexval, tt, 20)) && (getJsonValue(temp4, group, tt, 10)) && (getJsonValue(temp, condition, tt, 10))) {
                    try{
                        RuleTrigger rT;
                        rT.TriggerType = DeviceTriggers;
                        rT.DeviceID = atoi(devid);
                        rT.DeviceIndex = atoi(devindex);
                        strcpy(rT.IndexValue, indexval);
                        rT.TriggerGrouping = group[0] == 'A' ? AND : OR;
                        rT.ConditionType = stringToConditionenum(condition);
                        if (!checkForDeviceTriggerValidation(rT)) {
                            return false; // snprintf(buffer, 500, "{\"commandtype\":\"addtrigger.\",\"data\":{\"failure\":{\"ruleid\":\"%s\"}}}", ruleid);
                        }
                        rT.Valid = true;
                        sprintf(temp4, "Triggers[%d].Duration", i);
                        if (getJsonValue(temp4, duration, tt, 20)) {
                            rT.Duration = atoi(duration);
                        } else {
                            rT.Duration = 0;
                        }
                        Rules r(atoi(ruleid));
                        if (r.addTrigger(rT)) {
                            //return true;
                            checkupdate = TRUE;
                        } else {
                            checkupdate = FALSE;
                            break;
                            //return false;
                        }
                    } catch (int i) {
                        return false;
                    }
                } else {
                    return false;
                }

            } else if (!strcmp(triggertype, "EventTrigger")) {
                sprintf(temp, "Triggers[%d].Condition", i);
                sprintf(temp1, "Triggers[%d].ID", i);
                sprintf(temp2, "Triggers[%d].EventType", i);
                sprintf(temp3, "Triggers[%d].Value", i);
                sprintf(temp4, "Triggers[%d].Grouping", i);
                char eventid[10], eventype[50], eventval[50], group[10], condition[10], duration[20];
                if ((getJsonValue(temp1, eventid, tt, 10)) && (getJsonValue(temp2, eventype, tt, 50)) && (getJsonValue(temp3, eventval, tt, 50)) && (getJsonValue(temp4, group, tt, 10)) && (getJsonValue(temp, condition, tt, 10))) {
                    try{
                        RuleTrigger rT;
                        rT.TriggerType = EventTriggers;
                        rT.eventType = getEventTypeFromString(eventype);
                        rT.eventId = atoi(eventid);
                        strcpy(rT.eventValue, eventval);
                        rT.TriggerGrouping = group[0] == 'A' ? AND : OR;
                        rT.ConditionType = stringToConditionenum(condition);
                        rT.Valid = true;
                        sprintf(temp4, "Triggers[%d].Duration", i);
                        if (getJsonValue(temp4, duration, tt, 20)) {
                            rT.Duration = atoi(duration);
                        } else {
                            rT.Duration = 0;
                        }
                        Rules r(atoi(ruleid));
                        if (r.addTrigger(rT)) {
                            //  return true; //snprintf(buffer, 500, "{\"commandtype\":\"addtrigger.\",\"data\":{\"success\":{\"ruleid\":\"%s\"}}}", ruleid);
                            checkupdate = TRUE;
                        } else {
                            checkupdate = FALSE;
                            break;
                            //return false; //snprintf(buffer, 500, "{\"commandtype\":\"addtrigger.\",\"data\":{\"failure\":{\"ruleid\":\"%s\"}}}", ruleid);
                        }
                    } catch (int i) {
                        return false; // snprintf(buffer, 500, "{\"commandtype\":\"addtrigger.\",\"data\":{\"failure\":{\"ruleid\":\"%s\"}}}", ruleid);
                    }
                } else {
                    return false; //snprintf(buffer, 500, "{\"commandtype\":\"addtrigger.\",\"data\":{\"failure\":{\"ruleid\":\"%s\"}}}", ruleid);
                }
            } else if (!strcmp(triggertype, "TimeTrigger")) {
                sprintf(temp, "Triggers[%d].Hour", i);
                sprintf(temp1, "Triggers[%d].Minutes", i);
                sprintf(temp2, "Triggers[%d].DayOfMonth", i);
                sprintf(temp3, "Triggers[%d].MonthOfYear", i);
                sprintf(temp4, "Triggers[%d].DayOfWeek", i);
                sprintf(temp5, "Triggers[%d].Range", i);
                char mins[50], hours[50], range[50], monthofyear[50], dayofweek[15], dayofmonth[50], group[10];
                if ((getJsonValue(temp, hours, tt, 50)) && (getJsonValue(temp1, mins, tt, 50)) && (getJsonValue(temp5, range, tt, 50))&&(getJsonValue(temp3, monthofyear, tt, 50))&& (getJsonValue(temp4, dayofweek, tt, 15)) &&(getJsonValue(temp2, dayofmonth, tt, 50))) {
                    try{

                        RuleTrigger rT;
                        rT.TriggerType = TimeTriggers;
                        rT.timeTrigger.hours = atoi(hours);
                        rT.timeTrigger.mins = atoi(mins);
                        strcpy(rT.timeTrigger.DayOfMonth, dayofmonth);
                        strcpy(rT.timeTrigger.DayOfWeek, dayofweek);
                        strcpy(rT.timeTrigger.MonthOfYear, monthofyear);
                        rT.timeTrigger.range = atoi(range);
                        sprintf(temp5, "Triggers[%d].Grouping", i);
                        getJsonValue(temp5, group, tt, 10);
                        rT.TriggerGrouping = group[0] == 'A' ? AND : OR;
                        if (!checkForTimeTriggerValidation(rT)) {
                            return false; // snprintf(buffer, 500, "{\"commandtype\":\"addtrigger.\",\"data\":{\"failure\":{\"ruleid\":\"%s\"}}}", ruleid);
                        }
						rT.Duration = 0;
                        rT.Valid = true;
                        Rules r(atoi(ruleid));
                        if (r.addTrigger(rT)) {
                            //return true; //snprintf(buffer, 500, "{\"commandtype\":\"addtrigger.\",\"data\":{\"success\":{\"ruleid\":\"%s\"}}}", ruleid);
                            checkupdate = TRUE;
                        } else {
                            checkupdate = FALSE;
                            break;
                        }
                    } catch (int i) {
                        return false;
                    }
                } else {
                    return false;
                }

            } else if (!strcmp(triggertype, "WeatherTrigger")) {
                sprintf(temp, "Triggers[%d].WeatherType", i);
                sprintf(temp1, "Triggers[%d].Value", i);
                sprintf(temp2, "Triggers[%d].Grouping", i);
                sprintf(temp3, "Triggers[%d].Condition", i);
                       char weathertype[50], value[50], group[10], condition[10], duration[20];
                if ((getJsonValue(temp, weathertype, tt, 50))&& (getJsonValue(temp1, value, tt, 50))&& (getJsonValue(temp2, group, tt, 10)) && (getJsonValue(temp3, condition, tt, 10))) {
                           try{
                           LOG(Logger::INFO, "Weather trigger\n");
                               RuleTrigger rT;
                               rT.TriggerType = WeatherTriggers;
                        rT.weatherType = getWeatherTypeFromString(weathertype);
                               strcpy(rT.weatherValue, value);
                               rT.TriggerGrouping = group[0] == 'A' ? AND : OR;
                               rT.ConditionType = stringToConditionenum(condition);
                        sprintf(temp4, "Triggers[%d].Duration", i);
                        if (getJsonValue(temp4, duration, tt, 20)) {
                                   rT.Duration = atoi(duration);
                               } else {
                                   rT.Duration = 0;
                               }
                        rT.Valid = true;
                               Rules r(atoi(ruleid));
                               if (r.addTrigger(rT)) {

                            checkupdate = TRUE;
                        } else {
                            checkupdate = FALSE;
                            break;
                               }
                    } catch (int i) {
                               return false;
                           }
                       } else {
                    return false; // snprintf(buffer, 500, "{\"commandtype\":\"addtrigger.\",\"data\":{\"failure\":{\"ruleid\":\"%s\"}}}", ruleid);
                }
            } else if (!strcmp(triggertype, "VariableTrigger")) {
                sprintf(temp, "Triggers[%d].Condition", i);
                sprintf(temp1, "Triggers[%d].Name", i);
                sprintf(temp2, "Triggers[%d].Value", i);
                sprintf(temp3, "Triggers[%d].Grouping", i);
                int c=0;
                char varName[10], varValue[50], group[10], condition[10], duration[20], var='a',var1='a';
                if ((getJsonValue(temp1, varName, tt, 10)) && (getJsonValue(temp2, varValue, tt, 50)) && (getJsonValue(temp3, group, tt, 50)) && (getJsonValue(temp, condition, tt, 10))) {
                    if(isdigit(varValue[0]) == 0){
                        if(varValue[1]=='+' || varValue[1]=='-' || varValue[1]=='*' || varValue[1]=='/'){
                        if(isdigit(varValue[2]) == 0){
                            if(strlen(varValue) > 3)
                            {
                                LOG(Logger::INFO, "Add Trigger failed : Invalid value");
                                return false;
                            }
                            var1 = varValue[2];
                            c=2;
                        } else{   
                            var = varValue[0];
                            c=1;
                        }
                        }
                        else{
                            LOG(Logger::INFO, "Add Trigger failed : Invalid value 1");
                            return false;
                        }
                    }
                    if (strlen(varName) == 1 && ((int) varName[0] - 97) <= 19 && ((int) varName[0] - 97) >= 0 && ((int)var - 97)<=19 && ((int)var - 97)>=0 && ((int)var1 - 97)<=19 && ((int)var1 - 97)>=0) {
                        try{
                            RuleTrigger rT;
                            rT.TriggerType = VariableTriggers;
                            strcpy(rT.IndexValue, varName);
                            if(c==1 || c==2){
                                strcpy(rT.eventValue, varValue);
                            }else{
                                rT.DeviceIndex = atoi(varValue);
                            }
                            rT.eventId = c;
                            rT.TriggerGrouping = group[0] == 'A' ? AND : OR;
                            rT.ConditionType = stringToConditionenum(condition);
                            rT.Valid = true;
                            sprintf(temp4, "Triggers[%d].Duration", i);
                            if (getJsonValue(temp4, duration, tt, 20)) {
                                rT.Duration = atoi(duration);
                            } else {
                                rT.Duration = 0;
                            }
                            Rules r(atoi(ruleid));
                            if (r.addTrigger(rT)) {
                                checkupdate = TRUE;
                            } else {
                                checkupdate = FALSE;
                                break;
                            }
                        }

                        catch(int i) {
                            return false;
                        }
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        } else {
            //            return false;
            return true; //swetha 
            }
    }
    if (checkupdate) {
        return true;
    } else {
        return false;
    }


    return false;
}

bool AddResult_New(char *ruleid, struct json_token * tt) {
    char resulttype[50], devid[10], devindex[10], indexval[50], predelay[50];
    char temp[50] = "", temp1[50] = "", temp2[50] = "", temp3[50] = "";//, temp4[50] = "";

//    struct json_token *curr;
    struct json_token *curr;
    bool checkupdate = FALSE;
   
//    curr = 
    curr = find_json_token(tt, "Results");
    for (int i = 0; i < 100; i++) {
        sprintf(temp, "Results[%d].Type", i);
        if (getJsonValue(temp, resulttype, tt, 50)) {
            int ruleID = 0;
            ruleID = atoi(ruleid);
            if (!strcmp(resulttype, "DeviceResult")) {
                sprintf(temp, "Results[%d].PreDelay", i);
                sprintf(temp1, "Results[%d].ID", i);
                sprintf(temp2, "Results[%d].Index", i);
                sprintf(temp3, "Results[%d].Value", i);
                if ((getJsonValue(temp1, devid, tt, 10)) && (getJsonValue(temp2, devindex, tt, 10)) && (getJsonValue(temp3, indexval, tt, 50)) && (getJsonValue(temp, predelay, tt, 50))) {
                    try{
                        RuleResult rr;
                        rr.type = DeviceResult;
                        rr.DeviceID = atoi(devid);
                        rr.DeviceIndex = atoi(devindex);
                        strcpy(rr.IndexValue, indexval);
                        rr.PreDelay = atoi(predelay);
                        if (rr.PreDelay < 0) {
                            rr.PreDelay = 0;
                        }
                        if (!checkForDeviceResultValidation(rr)) {
                            return false;
                        }
                        rr.Valid = true;
                        Rules r(ruleID);
                        if (r.addResult(rr)) {
                            checkupdate = TRUE;
                        } else {
                            checkupdate = FALSE;
                            break;
                        }
                    } catch (int i) {
                        return false;
                    }
                } else {
                    return false;
                }
            } else if (!strcmp(resulttype, "EventResult")) {
                char eventid[10], eventtype[50], eventval[50], predelay[50];
                sprintf(temp, "Results[%d].PreDelay", i);
                sprintf(temp1, "Results[%d].ID", i);
                sprintf(temp2, "Results[%d].EventType", i);
                sprintf(temp3, "Results[%d].Value", i);
               // sprintf(temp4, "Results[%d].Validation", i);
                if ((getJsonValue(temp1, eventid, tt, 10)) && (getJsonValue(temp2, eventtype, tt, 50)) && (getJsonValue(temp3, eventval, tt, 50)) && (getJsonValue(temp, predelay, tt, 50))) {
                    try{
                        RuleResult rr;
                        rr.type = EventResult;
                        rr.eventType = getEventTypeFromString(eventtype);
                        rr.eventId = atoi(eventid);
                        strcpy(rr.eventValue, eventval);
                        rr.PreDelay = atoi(predelay);
                        if (rr.PreDelay < 0) {
                            rr.PreDelay = 0;
                        }
                        rr.Valid = true;
                        Rules r(ruleID);
                        if (r.addResult(rr)) {
                            checkupdate = TRUE;
                        } else {
                            checkupdate = FALSE;
                            break;
                        }
                    } catch (int i) {
                        return false;
                    }
                } else {
                    return false;
                }
            } else if (!strcmp(resulttype, "SceneResult")) {
                char sceneid[10], predelay[50];
                sprintf(temp, "Results[%d].PreDelay", i);
                sprintf(temp1, "Results[%d].ID", i);
                if ((getJsonValue(temp1, sceneid, tt, 10)) && (getJsonValue(temp, predelay, tt, 50))) {
                    try{

                        RuleResult rr;
                        rr.type = SceneResult;
                        rr.SceneID = atoi(sceneid);
                        rr.PreDelay = atoi(predelay);
                        if (rr.PreDelay < 0) {
                            rr.PreDelay = 0;
                        }
                        if (!checkForSceneResultValidation(rr)) {
                            return false;
                        }
                        rr.Valid = true;
                        Rules r(ruleID);
                        if (r.addResult(rr)) {
                            checkupdate = TRUE;
                        } else {
                            checkupdate = FALSE;
                            break;
                        }
                    } catch (int i) {
                        return false;
                    }
                } else {
                    return false;
                }

            } else if (!strcmp(resulttype, "NetworkResult")) {
                char networkvalue[180], predelay[50], networkid[5];
                sprintf(temp, "Results[%d].PreDelay", i);
                sprintf(temp1, "Results[%d].ID", i);
                sprintf(temp3, "Results[%d].Value", i);
                if ((getJsonValue(temp, predelay, tt, 50)) && (getJsonValue(temp3, networkvalue, tt, 180)) && (getJsonValue(temp1, networkid, tt, 5))) {
                    try{
                        RuleResult rr;
                        rr.type = NetworkResult;
                        rr.PreDelay = atoi(predelay);
                        strcpy(rr.eventValue, networkvalue);
                        rr.eventId = atoi(networkid);
                        if (rr.PreDelay < 0) {
                            rr.PreDelay = 0;
                        }
                        rr.Valid = true;
                        Rules r(ruleID);
                        if (r.addResult(rr)) {
                            checkupdate = TRUE;
                        } else {
                            checkupdate = FALSE;
                            break;
                        }
                    } catch (int i) {
                        return false;
                    }
                } else {
                    return false;
                }
            } else if (!strcmp(resulttype, "MessageResult")) {
                char msgid[10], msgtype[50], msgval[50], predelay[50];
                sprintf(temp, "Results[%d].PreDelay", i);
                sprintf(temp1, "Results[%d].ID", i);
                sprintf(temp2, "Results[%d].MessageType", i);
                sprintf(temp3, "Results[%d].Value", i);
                if ((getJsonValue(temp1, msgid, tt, 10)) && (getJsonValue(temp2, msgtype, tt, 50)) && (getJsonValue(temp3, msgval, tt, 50)) && (getJsonValue(temp, predelay, tt, 50))) {
                    try{
                        RuleResult rr;
                        rr.type = MessageResult;
                        rr.weatherType = getWeatherTypeFromString(msgtype);
                        rr.MsgID = atoi(msgid);
                        strcpy(rr.msgValue, msgval);
                        rr.PreDelay = atoi(predelay);
                        if (rr.PreDelay < 0) {
                            rr.PreDelay = 0;
                        }
                        switch (rr.weatherType) {
                            case Email:
                            {
                                if (!checkForEmailValidation(rr))
                                    return false;
                            }
                                break;
                            case SMS:
                            {
                                LOG(Logger::INFO, "-------Checking Phone Number--------");
                                if (!is_number(rr.msgValue)) {
                                    LOG(Logger::INFO, "-------Invalid Phone Number--------");
                                    return false;
                                }
                            }
                                break;
                            case PushMessage:
                            {
                                LOG(Logger::INFO, "Push Message:");
                                LOG(Logger::INFO, msgval);
                            }
                                break;
                            default:
                            {
                                LOG(Logger::INFO, "***Invalid Message type***");
                                return false;
                            }
                                break;
                        }
                        rr.Valid = true;
                        Rules r(ruleID);
                        if (r.addResult(rr)) {
                            checkupdate = TRUE;
                        } else {
                            checkupdate = FALSE;
                            break;
                        }
                    }

                    catch(int i) {
                        return false;
                    }
                }
            } else if (!strcmp(resulttype, "VariableResult")) {
                char id[10], name[10], value[50], predelay[50], var='a', var1='a';
                int c=0;
                sprintf(temp, "Results[%d].PreDelay", i);
                sprintf(temp1, "Results[%d].ID", i);
                sprintf(temp2, "Results[%d].Name", i);
                sprintf(temp3, "Results[%d].Value", i);
                if ((getJsonValue(temp1, id, tt, 10)) && (getJsonValue(temp2, name, tt, 10)) && (getJsonValue(temp3, value, tt, 50)) && (getJsonValue(temp, predelay, tt, 50))) {
                    if(isdigit(value[0]) == 0){
                        if(value[1]=='+' || value[1]=='-' || value[1]=='*' ||value[1]=='/'){
                        if(isdigit(value[2]) == 0){
                            if(strlen(value) > 3)
                            {
                                LOG(Logger::INFO, "Add result failed : Invalid value");
                                return false;
                            }
                            var1 = value[2];
                            c=2;
                        } else{   
                            var = value[0];
                            c=1;
                        }
                        }
                        else{
                            LOG(Logger::INFO, "Add result failed : Invalid value 1");
                            return false;
                        }
                    }
                    if (strlen(name) == 1 && ((int) name[0] - 97) <= 19 && ((int)var - 97)<=19 && ((int)var - 97)>=0 && ((int)var1 - 97)<=19 && ((int)var1 - 97)>=0 && ((int) name[0] - 97) >= 0) {
                        try{
                            RuleResult rr;
                            rr.type = VariableResult;
                            rr.DeviceID = atoi(id);
                            strcpy(rr.IndexValue, name);
                            rr.PreDelay = atoi(predelay);
                            if(c==1){
                                sscanf((char *)value, "%c%c%d", &rr.msgValue[0], &rr.eventValue[0], &rr.DeviceIndex);
                            }else if(c==2){
                                sscanf((char *)value, "%c%c%c", &rr.msgValue[0], &rr.eventValue[0], &rr.weathreValue[0]);
                            }else{
                                rr.DeviceIndex = atoi(value);
                            }
                            rr.MsgID = c;
                            if (rr.PreDelay < 0) {
                                rr.PreDelay = 0;
                            }
                            rr.Valid = true;
                            Rules r(ruleID);
                            if (r.addResult(rr)) {
                                checkupdate = TRUE;
                            } else {
                                checkupdate = FALSE;
                                break;
                            }
                        }

                        catch(int i) {
                            LOG(Logger::INFO, "Exception : Add result failed");
                            return false;
                        }
                    } else {
                        LOG(Logger::INFO, "Add result failed : Invalid Value 2");
                        return false;
                    }
                } else {
                    LOG(Logger::INFO, "Add result failed : Invalid Json");
                    return false;
                }
            }

            else {
                LOG(Logger::INFO, "Add result failed");
                return false;
            }

        } else {
            LOG(Logger::INFO, "Add result failed");
            return true; //swetha

        }
    }
    if (checkupdate) {
        return true;
    } else {
        return false;
    }
    return false;
}

bool gettriggerandresult_New(char * Num, char *ruleid, struct json_token *tt, int TrigRes) {
    bool var = false;
    if (TrigRes == 1) {
        LOG(Logger::INFO, "Adding trigger ");
        if (AddTrigger_New(ruleid, tt)) {
            var = true;
        } else {
            var = false;
        }
    } else if (TrigRes == 2) {
        LOG(Logger::INFO, "Adding Result");
        if (AddResult_New(ruleid, tt)) {
            var = true;
        } else {
            var = false;
        }
    }
    if (var)
        return true;
    else
        return false;
}

bool checkValidIndexTypeForRule(IndexType IT) {
    bool found = false;
    switch (IT) {
        case SwitchMultilevel:
        {
            found = true;
            break;
        }
        case DimmerLevel:
        {
            found = true;
            break;
        }
        case ShutterLevel:
        {
            found = true;
            break;
        }
        case LockStateZb:
        {
            found = true;
            break;
        }
        case Armmode:
        {
            found = true;
            break;
        }
        case Temperature:
        {
            found = true;
            break;
        }
        case Pressure:
        {
            found = true;
            break;
        }
        case Humidity:
        {
            found = true;
            break;
        }
        case HueValue:
        {
            found = true;
            break;
        }
        case HueZb:
        {
            found = true;
            break;
        }
        case Saturation:
        {
            found = true;
            break;
        }
        case ColorTemperature:
        {
            found = true;
            break;
        }
        case IRcode:
        {
            found = true;
            break;
        }

        case APower:
        case RmsVoltage:
        {
            found = true;
            break;
        }
        case RmsCurrent:
        {
            found = true;
            break;
        }
        case RetainState:
        {
            found = true;
            break;
        }
        case ResponseCode:
        {
            found = true;
            break;
        }
        case HueBulbId:
        {
            found = true;
            break;
        }
        default:
        {
            found = true;
            break;
        }

    }
    return found;
}

void getLuaDeviceEntry_as_json_New(Device &d, char* buf) {
    char lastknownbuf[512];
    char Name[200], Location[200];
    strcpy(Name, d.getDeviceName());
    strcpy(Name, specialchar(Name));
    strcpy(Location, d.getDeviceLocation());
    strcpy(Location, specialchar(Location));

   snprintf(buf, 1024, "{\"Data\":{\"ID\":\"%d\",\"Name\":\"%s\",\"FriendlyDeviceType\":\"%s\",\"Type\":\"%d\",\"Location\":\"%s\",\"LastActiveEpoch\":\"%llu\",\"Model\":\"%s\",\"Version\":\"%s\",\"Manufacturer\":\"%s\",\"DeviceState\":\"%d\"},\"DeviceValues\":{", d.getID(), Name, d.getFriendlyDeviceType(), d.getDeviceType(), Location,d.getLastActiveTime(),d.getModelid(),d.getVersion(),d.getManufacturername(),d.getdeviceState());
    int idx_cnt = d.getValueCount();

    for (int i = 0; i <= idx_cnt; i++) {

        int index = i;
        lastknownbuf[0] = '\0';
        if (i > 0) {
            snprintf(lastknownbuf, 100, ",\"%d\":{\"Name\":\"%s\",\"Value\":\"%s\",\"Type\":\"%d\"}", index, d.getValueName(index), d.getValue(index), d.getIndexType(index));
        } else {
            snprintf(lastknownbuf, 100, "\"%d\":{\"Name\":\"%s\",\"Value\":\"%s\",\"Type\":\"%d\"}", index, d.getValueName(index), d.getValue(index) ,d.getIndexType(index));
        }
        strcat(buf, lastknownbuf);
    }
    strcat(buf, "}}");
}

void getDeviceList_as_json_New(char *JSON) {
    DeviceList dl;

    char buf[2048] = "", local_buf[50] = "";
    bool Local = true;
    while (Local) {
        try{
            dl.Populate();
            Local = false;
        } catch (int ex) {
            char bufLog[200] = "";
            sprintf(bufLog, "exception caught while loading Device %d", ex);
            LOG(Logger::ERROR, bufLog);
            LOG(Logger::WARN, "Connected home Got Exception");
            if (ex != 0 && ex != -1) {
                //   DeleteCorruptedDevice(ex);
                LOG(Logger::WARN, "Device corrupted Exception");
            }
        }
    }

    list<Device>::iterator it;
    int i;
    sprintf(JSON, "{");

    for (i = 0, it = dl.devices.begin(); it != dl.devices.end(); ++it, ++i) {
        getLuaDeviceEntry_as_json_New(*it, buf);
        if (i > 0) {
            strcat(JSON, ",");
        }

        snprintf(local_buf, 50, "\"%d\":", it->getID());
        strcat(JSON, local_buf);
        strcat(JSON, buf);
    }
    strcat(JSON, "}");
}

void getLuaIndexEntry_as_json_New(Index &it,char* buf) {
    char temp1[1024];
    int i;
    snprintf(buf , 1024, "{\"Name\":\"%s\",\"DataType\":\"%s\",\"ReadOnly\":\"%s\",\"Layout\":\"%s\",\"Type\":\"%s\",\"GroupLabel\":\"%s\",\"DefaultIcon\":\"%s\",\"Placement\":\"%s\",\"SecondaryPlacement\":\"%s\",\"Conditional\":\"%s\",\"UseInScenes\":\"%s\",\"DefaultVisibility\":\"%s\",\"ShowToggleInRules\":\"%s\",\"ExcludeForm\":\"%s\",\"AppLabel\":\"%s\"",
                it.getIndexName(),it.getIndexDataType(),it.getIndexReadOnly(),it.getIndexLayout(),it.getIndexBehaviour(),it.getIndexGroupLabel(),it.getIndexDefaultIcon(),it.getIndexPlacement(),it.getIndexSPlacement(),it.getIndexConditional(),it.getIndexUseinScenes(),it.getIndexDefaultVisibility(),it.getIndexShowToggleInRules(),it.getIndexExcludeFrom(),it.getIndexAppLabel());
    
    if(it.ValueCount == 0){
        snprintf(temp1,1024,",\"Formatter\":{\"Min\":\"%s\",\"Max\":\"%s\",\"Unit\":\"%s\",\"Factor\":\"%s\"}",it.getIndexMin(),it.getIndexMax(),it.getIndexUnits(),it.getIndexFactor());
        strcat(buf,temp1);
       // LOG(Logger::INFO, "IndexList Post");
    }
    else{
        strcat(buf,",\"Values\":{");
        for(i=0 ; i<it.ValueCount;i++){
           char temp[1024];
            if(i>0){
                snprintf(temp , 500 , ",\"%s\":{\"Label\":\"%s\",\"Icon\":\"%s\",\"ToggleValue\":\"%s\",\"AppLabel\":\"%s\",\"ExcludeFrom\":\"%s\"}",
                            it.getValue(i+1),it.getValueLabel(i+1),it.getValueIcon(i+1),it.getValueToggleValue(i+1),it.getValueAppLabel(i+1),it.getValueExcludeFrom(i+1));
            }
            else{
                snprintf(temp , 500 , "\"%s\":{\"Label\":\"%s\",\"Icon\":\"%s\",\"ToggleValue\":\"%s\",\"AppLabel\":\"%s\",\"ExcludeFrom\":\"%s\"}",
                            it.getValue(i+1),it.getValueLabel(i+1),it.getValueIcon(i+1),it.getValueToggleValue(i+1),it.getValueAppLabel(i+1),it.getValueExcludeFrom(i+1));
            }
           //LOG(Logger::INFO, temp);
            strcat(buf,temp);
        }
        
        strcat(buf,"}");
    }
    strcat(buf,"}");
}




void getIndexList_as_json_New(char* JSON) {
    
    IndexList il;
    //int counter = 0;
    char buf[3072] = "";
            char local_buf[50] = "";
    bool Local = true;
    while (Local) {
        try{
            il.Populate();
            Local = false;
        }

        catch(int ex) {
            char bufLog[200] = "";
            sprintf(bufLog, "exception caught while loading Index %d", ex);
            LOG(Logger::ERROR, bufLog);
            LOG(Logger::WARN, "Connected home Got Exception");
            //if (ex != 0 && ex != -1) {
            //    DeleteCorruptedDevice(ex);
            //}
        }
    }

    list<Index>::iterator it;
    int i;
    sprintf(JSON, "{");
    
    for (i = 0, it = il.IndexValueList.begin(); it != il.IndexValueList.end(); ++it, ++i) {
//        counter =counter+1;
        getLuaIndexEntry_as_json_New(*it, buf);
        if (i > 0) {
            strcat(JSON, ",");
        }

        snprintf(local_buf, 50, "\"%d\":", it->getIndexID());
        strcat(JSON, local_buf);
        strcat(JSON, buf);
    }
    strcat(JSON, "}");
}
char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//static inline bool is_base64(unsigned char c) {
//    return (isalnum(c) || (c == '+') || (c == '/'));
//}

void encodeblock(unsigned char in[], char b64str[], int len) {
    char out[5];
    out[0] = b64[ in[0] >> 2 ];
    out[1] = b64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? b64[ ((in[1] & 0x0f) << 2) |
            ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? b64[ in[2] & 0x3f ] : '=');
    out[4] = '\0';
    strncat(b64str, out, sizeof (out));
}

void b64_encode(char *clrstr, char *b64dst) {
    unsigned char in[3];
    int i, len = 0;
    int j = 0;
    b64dst[0] = '\0';
    while (clrstr[j]) {
        len = 0;
        for (i = 0; i < 3; i++) {
            in[i] = (unsigned char) clrstr[j];
            if (clrstr[j]) {
                len++;
                j++;
            } else in[i] = 0;
        }
        if (len) {
            encodeblock(in, b64dst, len);
        }
    }
}

void decodeblock(unsigned char in[], char *clrstr) {
    char out[4];
    out[0] = in[0] << 2 | in[1] >> 4;
    out[1] = in[1] << 4 | in[2] >> 2;
    out[2] = in[2] << 6 | in[3] >> 0;
    out[3] = '\0';
    strncat(clrstr, out, sizeof (out));
}

void b64_decode(char *b64src, char *clrdst) {
    int c, phase, i;
    unsigned char in[4];
    char *p;

    clrdst[0] = '\0';
    phase = 0;
    i = 0;
    while (b64src[i]) {
        c = (int) b64src[i];
        if (c == '=') {
            decodeblock(in, clrdst);
            break;
        }
        p = strchr(b64, c);
        if (p) {
            in[phase] = p - b64;
            phase = (phase + 1) % 4;
            if (phase == 0) {
                decodeblock(in, clrdst);
                in[0] = in[1] = in[2] = in[3] = 0;
            }
        }
        i++;
    }
}
