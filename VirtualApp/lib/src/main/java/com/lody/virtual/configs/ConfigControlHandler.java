package com.lody.virtual.configs;

import com.lody.virtual.os.VEnvironment;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.util.HashMap;

public class ConfigControlHandler {
    public static final String CAMARA_KEY = "isEnableCamara";
    public static final String CLIPBOARD_KEY = "isEnableClipBoard";
    public static final String STOREENCRYPTED_KEY = "isEnableStoreEncrypted";
    private static HashMap<String, ConfigControl> configControlHashMap;
    private static ConfigControlHandler instance;

    public static ConfigControlHandler getInstance() {
        if (instance == null) {
            instance = new ConfigControlHandler();
            configControlHashMap = new HashMap<>();
        }
        return instance;
    }

    public ConfigControl getAppConfigControl(String packagename) {
        ConfigControl configControl = configControlHashMap.get(packagename);
        if (configControl == null) {
            return null;
        }
        return configControl;
    }


    public ConfigControl putAppConfigControl(String packagename, ConfigControl configControl) {
        if (configControlHashMap == null) {
            configControlHashMap = new HashMap<>();
        }
        return configControlHashMap.put(packagename, configControl);
    }

    public void saveFile() {
        JSONArray jsonArray = new JSONArray();
        for (String key : configControlHashMap.keySet()) {
            ConfigControl configControl = configControlHashMap.get(key);
            JSONObject jsonObject = new JSONObject();
            try {
                jsonObject.put("enableClipboard", configControl.enableClipboard);
                jsonObject.put("enableCamera", configControl.enableCamera);
                jsonObject.put("enableStoreEncrypted", configControl.enableStoreEncrypted);
                jsonObject.put("packagename", key);
            } catch (JSONException e) {
                e.printStackTrace();
            }
            jsonArray.put(jsonObject);
        }
        String jsString = jsonArray.toString();
        try {
            FileOutputStream fileOutputStream = new FileOutputStream(VEnvironment.getConfigFile());
            fileOutputStream.write(jsString.getBytes());
            fileOutputStream.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    public ConfigControl readFile(String packageName) {
        ConfigControl configControl = getDefaultConfigControl();
        try {
            FileInputStream fileInputStream = new FileInputStream(VEnvironment.getConfigFile());
            if (fileInputStream == null) {
                return configControl;
            }
            String content = "";

            byte[] cnb = new byte[144];
            while (fileInputStream.read(cnb) != -1) {
                content += new String(cnb);
            }
            fileInputStream.close();
            JSONArray jsonArray = new JSONArray(content);
            if(jsonArray.length()<=0){
                return configControl;
            }
            configControlHashMap.clear();
            for(int i =0;i<jsonArray.length();i++){
                JSONObject jsonObject = jsonArray.getJSONObject(i);
                if(jsonObject!=null){
                    ConfigControl config= new ConfigControl();
                    config.packagename = jsonObject.getString("packagename");
                    config.enableClipboard = jsonObject.getBoolean("enableClipboard");
                    config.enableCamera = jsonObject.getBoolean("enableCamera");
                    config.enableStoreEncrypted = jsonObject.getBoolean("enableStoreEncrypted");
                    configControlHashMap.put(config.packagename,config);
                }
            }
            if(configControlHashMap.containsKey(packageName)){
                return configControlHashMap.get(packageName);
            }else {
                return configControl;
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return configControl;
    }


    private ConfigControl getDefaultConfigControl(){
        ConfigControl configControl = new ConfigControl();
        configControl.enableClipboard = true;
        configControl.enableCamera = true;
        configControl.enableStoreEncrypted = false;
        return configControl;
    }

    public class ConfigControl implements Serializable {
        String packagename;
        boolean enableClipboard;//是否启用剪贴板
        boolean enableCamera;//是否启用摄像头
        boolean enableStoreEncrypted;//是否启用存储加密

        public boolean isEnableClipboard() {
            return enableClipboard;
        }

        public boolean isEnableCamera() {
            return enableCamera;
        }

        public boolean isEnableStoreEncrypted() {
            return enableStoreEncrypted;
        }

        public void setEnableClipboard(boolean enableClipboard) {
            this.enableClipboard = enableClipboard;
        }

        public void setEnableCamera(boolean enableCamera) {
            this.enableCamera = enableCamera;
        }

        public void setEnableStoreEncrypted(boolean enableStoreEncrypted) {
            this.enableStoreEncrypted = enableStoreEncrypted;
        }
    }
}
