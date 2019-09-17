/* 
 * File:   test-echo.h
 * Author: swetha
 *
 * Created on 29 September, 2015, 3:27 PM
 */

#ifndef TEST_ECHO_H
#define	TEST_ECHO_H

#define GET_SETTING     	1
#define GET_NOSAVESETTING	2
#define SAVE_SETTING		3
#define ADD_CONFIG		4
#define DEL_SETTING		5
#define	ADD_VALUE		6
#define	SAVE_VALUE		7
#define COMMIT			8
#define CREATE_SETTING_FILE	9
#define GETMAC			10
#define GETCLOUDID		11  

int getmemoryfunction(char* str) {
    int ret = 0;
    if (!strcmp(str, "getSetting")) {
        ret = GET_SETTING;
        return ret;
    } else if (!strcmp(str, "getNoSaveSetting")) {
        ret = GET_NOSAVESETTING;
        return ret;
    } else if (!strcmp(str, "saveSetting")) {
        ret = SAVE_SETTING;
        return ret;
    } else if (!strcmp(str, "addConfig")) {
        ret = ADD_CONFIG;
        return ret;
    } else if (!strcmp(str, "delSetting")) {
        ret = DEL_SETTING;
        return ret;
    } else if (!strcmp(str, "addValue")) {
        ret = ADD_VALUE;
        return ret;
    } else if (!strcmp(str, "saveValues")) {
        ret = SAVE_VALUE;
        return ret;
    } else if (!strcmp(str, "commit")) {
        ret = COMMIT;
        return ret;
    } else if (!strcmp(str, "createSettingsFile")) {
        ret = CREATE_SETTING_FILE;
        return ret;
    } else if (!strcmp(str, "getMac")) {
        ret = GETMAC;
        return ret;
    } else if (!strcmp(str, "getCloudID")) {
        ret = GETCLOUDID;
        return ret;
    }
    return 0;
}

#define DEVICE_LIST             1
#define SET_DEVICE              2
#define GET_DEVICE              3
#define EDIT_DEVICE_NAME        4
#define ALMOND_MODE             5

#define RULES_LIST              6
#define CREATE_RULE             7
#define REMOVE_RULE             8

#define ADD_TRIGGER             9
#define REMOVE_TRIGGER          10
#define REMOVE_ALL_TRIGGERS     11

#define ADD_RESULT              12
#define REMOVE_RESULT           13
#define REMOVE_ALL_RESULTS      14

#define VALIDATE_RULE           15
#define SET_PREDELAY            16
#define SET_RULE_NAME           17
#define DELETE_ALL_RULES        18
#define SET_POSTDELAY           19

#define NEW_RULE                20
#define EDIT_RULE               21

#define CLIENTSLIST                 22
#define CLIENTUPDATE                23
#define REMOVE_CLIENT               24
#define REMOVE_ALL_CLIENTS          25

#define SCENES_LIST                 26
#define CREATE_SCENE                27
#define SET_SCENE                   28
#define ACTIVATE_SCENE              29
#define REMOVE_SCENE                30
#define REMOVE_ALL_SCENES           31

#define SET_ALMOND_NAME             32
#define GET_MAC_NAME                33
//#define GET_ROUTER_SUMMARY          34
#define MEMORYCLASS                 35
#define SYSTEMCOMMAND               36
#define SITE_SURVEY                 37
#define CHECK_INTERNET              38
#define CHECK_CABLES                39
#define CHECK_WAN_CABLE             40
#define CHECK_CABLE_INTERNET        41
#define GET_SSID_AND_PASSWORD       42
#define SET_SSID_AND_PASSWORD       43
#define ENABLE_RANGE_EXTENDER       44
#define ENABLE_SSID3                45
#define DISABLE_RANGE_EXTENDER      46
#define IS_RANGE_EXTENDER_ENABLED   47

#define RULE_LIST                   48
#define ADD_RULE                    49
#define UPDATE_RULE                 50
#define SET_RULE                    51
#define REMOVE_RULE_1               52
#define DELETE_ALL_RULES_1          53

#define ADD_DEVICE_1                  54
#define REMOVE_DEVICE               55
#define REMOVE_ALL_DEVICES          56
#define CANCEL_PROCESS              57
#define ASSOC_SET_NAME              58
#define SELECT_DEVICETYPE           59
#define DELETE_DEVICE               60

#define CHECK_ASSOCIATION           61
#define SEARCH_DEVICES              62
#define ADD_IPDEVICES               63
#define REMOVE_IPDEVICES            64
#define REFRESH_DEVICES             65
#define ADD_DEVICES                 66
#define CHECK_NEST_EXIST            67
#define INDEX_LIST                  68

#define ROUTER_SUMMARY              69
#define GET_WIRELESS_SETTINGS       70
#define SET_WIRELESS_SETTINGS       71
#define ROUTER_REBOOT               72
#define SEND_LOGS                   73
#define FIRMWARE_UPDATE             74

#define ALMOND_PROPERTIES           75
#define SET_RANGE_EXTENDER          76
#define ASSOC_GET_NAME              77

#define  DEVICELIST                 78
#define UPDATE_DEVICE_INDEX         79
#define GET_DEVICE_INDEX            80
#define UPDATE_DEVICE_NAME          81
#define UPDATE_ALMOND_MODE          82
#define VARIABLE_LIST               83
#define DISPLAY                     84

int getCmdValue(char* str) {
    int ret = 0;
    if (!strcmp(str, "devicelist")) {
        ret = DEVICE_LIST;
        return ret;
    } else if (!strcmp(str, "setdeviceindex")) {
        ret = SET_DEVICE;
        return ret;
    } else if (!strcmp(str, "getdeviceindex")) {
        ret = GET_DEVICE;
        return ret;
    } else if (!strcmp(str, "editdevicename")) {
        ret = EDIT_DEVICE_NAME;
        return ret;
    } else if (!strcmp(str, "updatealmondmode")) {
        ret = ALMOND_MODE;
        return ret;
    } else if (!strcmp(str, "ruleslist")) {
        ret = RULES_LIST;
        return ret;
    } else if (!strcmp(str, "createrule")) {   
        ret = CREATE_RULE;
        return ret;
    } else if (!strcmp(str, "removerule")) {
        ret = REMOVE_RULE;
        return ret;
    } else if (!strcmp(str, "addtrigger")) {
        ret = ADD_TRIGGER;
        return ret;
    } else if (!strcmp(str, "removetrigger")) {
        ret = REMOVE_TRIGGER;
        return ret;
    } else if (!strcmp(str, "removealltriggers")) {
        ret = REMOVE_ALL_TRIGGERS;
        return ret;
    } else if (!strcmp(str, "addresult")) {
        ret = ADD_RESULT;
        return ret;
    } else if (!strcmp(str, "removeresult")) {
        ret = REMOVE_RESULT;
        return ret;
    } else if (!strcmp(str, "removeallresults")) {
        ret = REMOVE_ALL_RESULTS;
        return ret;
    } else if (!strcmp(str, "validaterule")) {
        ret = VALIDATE_RULE;
        return ret;
    } else if (!strcmp(str, "setpredelay")) {
        ret = SET_PREDELAY;
        return ret;
    } else if (!strcmp(str, "setrulename")) {
        ret = SET_RULE_NAME;
        return ret;
    } else if (!strcmp(str, "deleteallrules")) {
        ret = DELETE_ALL_RULES;
        return ret;
    } else if (!strcmp(str, "setpostdelay")) {
        ret = SET_POSTDELAY;
        return ret;
    } else if (!strcmp(str, "newrule")) {
        ret = NEW_RULE;
        return ret;
    } else if (!strcmp(str, "editrule")) {
        ret = EDIT_RULE;
        return ret;
    } else if (!strcmp(str, "ClientList")) {
        ret = CLIENTSLIST;
        return ret;
    } else if (!(strcmp(str, "UpdateClient"))) {
  
        ret = CLIENTUPDATE;
        return ret;
    } else if (!strcmp(str, "RemoveClient")) {
        ret = REMOVE_CLIENT;
        return ret;
    } else if (!strcmp(str, "RemoveAllClients")) {
        ret = REMOVE_ALL_CLIENTS;
        return ret;
    } else if (!strcmp(str, "DynamicSceneList")) {
        ret = SCENES_LIST;
        return ret;
    } else if (!strcmp(str, "AddScene")) {
        ret = CREATE_SCENE;
        return ret;
    } else if (!strcmp(str, "UpdateScene")) {
        ret = SET_SCENE;
        return ret;
    } else if (!strcmp(str, "ActivateScene")) {
        ret = ACTIVATE_SCENE;
        return ret;
    } else if (!strcmp(str, "RemoveScene")) {
        ret = REMOVE_SCENE;
        return ret;
    } else if (!strcmp(str, "RemoveAllScenes")) {
        ret = REMOVE_ALL_SCENES;
        return ret;
    } else if (!strcmp(str, "SetAlmondName")) {
        ret = SET_ALMOND_NAME;
        return ret;
    } else if (!strcmp(str, "GetAlmondNameAndMAC")) {
        ret = GET_MAC_NAME;
        return ret;
    } else if (!strcmp(str, "RouterSummary")) {
        ret = ROUTER_SUMMARY;
        return ret;
    } else if (!strcmp(str, "GetWirelessSettings")) {
        ret = GET_WIRELESS_SETTINGS;
        return ret;
    } else if (!strcmp(str, "SetWirelessSettings")) {
        ret = SET_WIRELESS_SETTINGS;
        return ret;
    } else if (!strcmp(str, "RebootRouter")) {
        ret = ROUTER_REBOOT;
        return ret;
    } else if (!strcmp(str, "SendLogs")) {
        ret = SEND_LOGS;
        return ret;
    } else if (!strcmp(str, "FirmwareUpdate")) {
        ret = FIRMWARE_UPDATE;
        return ret;
    } else if (!strcmp(str, "AlmondProperties")) {
        ret = ALMOND_PROPERTIES;
        return ret;
    } else if (!strcmp(str, "RouterSettings")) {
        ret = MEMORYCLASS;
        return ret;
    } else if (!strcmp(str, "SystemCommand")) {
        ret = SYSTEMCOMMAND;
        return ret;
    } else if (!strcmp(str, "CheckInternet")) {
        ret = CHECK_INTERNET;
        return ret;
    } else if (!strcmp(str, "CheckCables")) {
        ret = CHECK_CABLES;
        return ret;
    } else if (!strcmp(str, "CheckWANCable")) {
        ret = CHECK_WAN_CABLE;
        return ret;
    } else if (!strcmp(str, "SiteSurvey")) {
        ret = SITE_SURVEY;
        return ret;
    } else if (!strcmp(str, "CheckCableAndInternet")) {
        ret = CHECK_CABLE_INTERNET;
        return ret;
    } else if (!strcmp(str, "GetSSIDAndPassword")) {
        ret = GET_SSID_AND_PASSWORD;
        return ret;
    } else if (!strcmp(str, "SetSSIDAndPassword")) {
        ret = SET_SSID_AND_PASSWORD;
        return ret;
    } else if (!strcmp(str, "EnableRangeExtender")) {
        ret = ENABLE_RANGE_EXTENDER;
        return ret;
    } else if (!strcmp(str, "EnableSSID3")) {
        ret = ENABLE_SSID3;
        return ret;
    } else if (!strcmp(str, "DisableRangeExtender")) {
        ret = DISABLE_RANGE_EXTENDER;
        return ret;
    } else if (!strcmp(str, "isRangeExtenderEnabled")) {
        ret = IS_RANGE_EXTENDER_ENABLED;
        return ret;
    } else if (!strcmp(str, "RuleList")) {
        ret = RULE_LIST;
        return ret;
    } else if (!strcmp(str, "AddRule")) {
        return ADD_RULE;
    } else if (!strcmp(str, "UpdateRule")) {
        return UPDATE_RULE;
    } else if (!strcmp(str, "ValidateRule")) {
        return SET_RULE;
    } else if (!strcmp(str, "RemoveRule")) {
        return REMOVE_RULE_1;
    } else if (!strcmp(str, "RemoveAllRules")) {
        return DELETE_ALL_RULES_1;
    } else if (!strcmp(str, "AddDevice")) {
        ret = ADD_DEVICE_1;
        return ret;
    } else if (!strcmp(str, "RemoveDevice")) {
        ret = REMOVE_DEVICE;
        return ret;
    } else if (!strcmp(str, "RemoveAllDevices")) {
        ret = REMOVE_ALL_DEVICES;
        return ret;
    } else if (!strcmp(str, "CancelProcess")) {
        return CANCEL_PROCESS;
    } else if (!strcmp(str, "SetName")) {
        return ASSOC_SET_NAME;
    } else if (!strcmp(str, "SelectDevicetype")) {
        return SELECT_DEVICETYPE;
    } else if (!strcmp(str, "DeleteDevice")) {
        return DELETE_DEVICE;
    }  else if (!strcmp(str, "CheckAssociation")) {
        return CHECK_ASSOCIATION;
    } else if (!strcmp(str, "SearchDevice")) {
        return SEARCH_DEVICES;
    } else if (!strcmp(str, "AddIPDevice")) {
        return ADD_IPDEVICES;
    } else if (!strcmp(str, "RemoveIPDevice")) {
        return REMOVE_IPDEVICES;
    } else if (!strcmp(str, "RefreshDevice")) {
        return REFRESH_DEVICES;
    } else if (!strcmp(str, "CheckNestExist")) {
        return CHECK_NEST_EXIST;
    }else if (!strcmp(str, "DeviceList")) {
        ret = DEVICELIST;
        return ret;
    } else if (!strcmp(str, "UpdateDeviceIndex")) {
        ret = UPDATE_DEVICE_INDEX;
        return ret;
    } else if (!strcmp(str, "GetDeviceIndex")) {
        ret = GET_DEVICE_INDEX;
        return ret;
    } else if (!strcmp(str, "UpdateDeviceName")) {
        ret = UPDATE_DEVICE_NAME;
        return ret;
    } else if (!strcmp(str, "UpdateAlmondMode")) {
        ret = UPDATE_ALMOND_MODE;
        return ret;
    } else if (!strcmp(str, "VariableList")) {
        ret = VARIABLE_LIST;
        return ret;
    }else if(!strcmp(str, "IndexList")) {
        ret = INDEX_LIST;
        return ret;
    }
    else if(!strcmp(str,"Display")){
       ret = DISPLAY;
       return ret;
    }
    return 0;
}

char *decodestring(char *s) {
    if (strstr(s, "&bsol;")) {
        char temp[500], temp_1[500];
        strcpy(temp_1, s);
        strcpy(temp, s);
        *strstr(temp_1, "&bsol;") = '\0';
        sprintf(s, "%s\\%s", temp_1, (strstr(temp, "&bsol;") + 6));
        decodestring(s);
    } else if (strstr(s, "&quot;")) {
        char temp[500], temp_1[500];
        strcpy(temp_1, s);
        strcpy(temp, s);
        *strstr(temp_1, "&quot;") = '\0';
        sprintf(s, "%s\"%s", temp_1, (strstr(temp, "&quot;") + 6));
        decodestring(s);
    }
    return s;
}
char *decodepassword(char *s) {
    if (strstr(s, "&hash;")) {
        char temp[500], temp_1[500];
        strcpy(temp_1, s);
        strcpy(temp, s);
        *strstr(temp_1, "&hash;") = '\0';
        sprintf(s, "%s#%s", temp_1, (strstr(temp, "&hash;") + 6));
        decodestring(s);
    } else if (strstr(s, "&percent;")) {
        char temp[500], temp_1[500];
        strcpy(temp_1, s);
        strcpy(temp, s);
        *strstr(temp_1, "&percent;") = '\0';
        sprintf(s, "%s%%%s", temp_1, (strstr(temp, "&percent;") + 9));
        decodestring(s);
    } else if (strstr(s, "&slash;")) {
        char temp[500], temp_1[500];
        strcpy(temp_1, s);
        strcpy(temp, s);
        *strstr(temp_1, "&slash;") = '\0';
        sprintf(s, "%s\\%s", temp_1, (strstr(temp, "&slash;") + 7));
        decodestring(s);
        return s;
    } else if (strstr(s, "&quest;")) {
        char temp[500], temp_1[500];
        strcpy(temp_1, s);
        strcpy(temp, s);
        *strstr(temp_1, "&quest;") = '\0';
        sprintf(s, "%s?%s", temp_1, (strstr(temp, "&quest;") + 7));
        decodestring(s);
        return s;
    }
    return s;
}



char *specialchar(char *s) {
    if ((strchr(s, '"'))) {
        char temp[200], temp_1[200];
        strcpy(temp, s);
        strcpy(temp_1, s);
        *(strchr(temp, '"')) = '\0';
        sprintf(s, "%s&quot;%s", temp, strchr(temp_1, '"') + 1);
        specialchar(s);
    } else if ((strchr(s, '\\'))) {
        char temp[200], temp_1[200];
        strcpy(temp, s);
        strcpy(temp_1, s);
        *(strchr(temp, '\\')) = '\0';
        sprintf(s, "%s&bsol;%s", temp, strchr(temp_1, '\\') + 1);
        specialchar(s);
    } else {
        return s;
    }
    return s;
}
#endif	/* TEST_ECHO_H */

