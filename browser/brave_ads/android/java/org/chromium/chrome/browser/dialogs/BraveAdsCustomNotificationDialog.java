/** Copyright (c) 2019 The Brave Authors. All rights reserved.
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
  * You can obtain one at http://mozilla.org/MPL/2.0/.
  */

package org.chromium.chrome.browser.dialogs;

import android.app.AlarmManager;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager.NameNotFoundException;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.TextView;

import org.chromium.base.ContextUtils;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.BraveActivity;
import org.chromium.chrome.browser.BraveAdsNativeHelper;
import org.chromium.chrome.browser.BraveFeatureList;
import org.chromium.chrome.browser.BraveRewardsNativeWorker;
import org.chromium.chrome.browser.BraveRewardsPanelPopup;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.notifications.BraveOnboardingNotification;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.settings.BraveRewardsPreferences;
import org.chromium.chrome.browser.preferences.BravePref;
import org.chromium.chrome.browser.preferences.PrefServiceBridge;
import org.chromium.chrome.browser.util.PackageUtils;

import java.lang.System;

public class BraveAdsCustomNotificationDialog {

    static AlertDialog mAdsDialog;
    static String mNotificationId;

    @CalledByNative
    private static void displayAdsCustomNotification(final String notificationId,
            final String origin, final String title, final String body) {
        Context context = ContextUtils.getApplicationContext();
        AlertDialog.Builder b = new AlertDialog.Builder(context);

        LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        b.setView(inflater.inflate(R.layout.brave_ads_custom_notification, null));
        mAdsDialog = b.create();

        Window window = mAdsDialog.getWindow();
        WindowManager.LayoutParams wlp = window.getAttributes();

        ((TextView) mAdsDialog.findViewById(R.id.brave_ads_custom_notification_header)).setText(title);
        ((TextView) mAdsDialog.findViewById(R.id.brave_ads_custom_notification_body)).setText(body);
        mAdsDialog.show();

        wlp.gravity = Gravity.TOP;
        wlp.dimAmount = 0.0f;
        wlp.flags |= WindowManager.LayoutParams.FLAG_DIM_BEHIND;

        mAdsDialog.setCanceledOnTouchOutside(false);
        mAdsDialog.setCancelable(false);

        window.setAttributes(wlp);
        ImageView closeButton = mAdsDialog.findViewById(R.id.brave_ads_custom_notification_close_button);

        mNotificationId = notificationId;
        closeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // mAdsDialog.hide();
                mAdsDialog.hide();
                BraveAdsNativeHelper.nativeAdDismissed(Profile.getLastUsedRegularProfile());
                // BraveAdsNativeHelper.nativeAdDismissed(Profile.getLastUsedRegularProfile(), mNotificationId);
            }
        });

        mAdsDialog.findViewById(R.id.brave_ads_custom_notification_popup).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                BraveActivity.getBraveActivity().openNewOrSelectExistingTab(origin);
                mAdsDialog.hide();
                BraveAdsNativeHelper.nativeAdClicked(Profile.getLastUsedRegularProfile());
//                BraveAdsNativeHelper.nativeAdClicked(Profile.getLastUsedRegularProfile(), mNotificationId);
            }
        });
    }

    @CalledByNative
    private static void closeAdsCustomNotification(final String notificationId) {
        if (mNotificationId != null && mNotificationId.equals(notificationId) && mAdsDialog != null) {
            mAdsDialog.hide();
        }
    }
}
