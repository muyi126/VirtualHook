package io.virtualapp.vs;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.util.Log;
import android.widget.CompoundButton;
import android.widget.Switch;

import com.lody.virtual.configs.ConfigControlHandler;

import io.virtualapp.abs.ui.VActivity;
import io.virtualhook.R;

/**
 * @author Lody
 */
public class VSManagerActivity extends VActivity {
    Switch mSwCamara;
    Switch mSwClipBorad;
    Switch mSwStoreEncrypted;

    //    boolean enableCamara;
//    boolean enableClipBoard;
//    boolean enableStoreEncrypted;
    private ConfigControlHandler.ConfigControl allConfig;

    public static void goManager(Context context) {
        Intent intent = new Intent(context, VSManagerActivity.class);
        context.startActivity(intent);
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_manager_layout);
        initData();
        initView();

    }

    CompoundButton.OnCheckedChangeListener listener = new CompoundButton.OnCheckedChangeListener() {
        @Override
        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
            switch (buttonView.getId()) {
                case R.id.sw_camara:
                    allConfig.setEnableCamera(isChecked);
                    break;
                case R.id.sw_clipBoard:
                    allConfig.setEnableClipboard(isChecked);
                    break;
                case R.id.sw_storeEncrypted:
                    allConfig.setEnableStoreEncrypted(isChecked);
                    break;
            }
        }
    };


    void initView() {
        mSwCamara = (Switch) findViewById(R.id.sw_camara);
        mSwClipBorad = (Switch) findViewById(R.id.sw_clipBoard);
        mSwStoreEncrypted = (Switch) findViewById(R.id.sw_storeEncrypted);
        mSwClipBorad.setChecked(allConfig.isEnableClipboard());
        mSwCamara.setChecked(allConfig.isEnableCamera());
        mSwStoreEncrypted.setChecked(allConfig.isEnableStoreEncrypted());


        mSwClipBorad.setOnCheckedChangeListener(listener);
        mSwCamara.setOnCheckedChangeListener(listener);
        mSwStoreEncrypted.setOnCheckedChangeListener(listener);
    }

    void initData() {
        ConfigControlHandler instance = ConfigControlHandler.getInstance();
        allConfig = instance.readFile("all");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        ConfigControlHandler instance = ConfigControlHandler.getInstance();
        Log.i("AA", "onDestroy " + instance.toString());
        instance.putAppConfigControl("all", allConfig);
        instance.saveFile();

    }
}
