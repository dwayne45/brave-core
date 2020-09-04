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
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import org.chromium.base.ContextUtils;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.chrome.R;
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
    @CalledByNative
    private static void displayAdsCustomNotification(final String notificationId,
            final String origin, final String title, final String body) {
      Log.i("albert", "Called display ads custom notification");
      Toast.makeText(ContextUtils.getApplicationContext(), "title: " + title + " body: " + body, Toast.LENGTH_LONG);
    }

    /*
    public static void showNewUserDialog(Context context) {
        AlertDialog alertDialog = new AlertDialog.Builder(context, R.style.BraveDialogTheme)
        .setView(R.layout.brave_ads_new_user_dialog_layout)
        .setPositiveButton(R.string.brave_ads_offer_positive, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {

                OnboardingPrefManager.getInstance().setOnboardingNotificationShown(false);

                neverShowOnboardingDialogAgain();

                BraveRewardsNativeWorker braveRewardsNativeWorker = BraveRewardsNativeWorker.getInstance();
                braveRewardsNativeWorker.GetRewardsMainEnabled();
                braveRewardsNativeWorker.CreateWallet();

                // Enable ads
                BraveAdsNativeHelper.nativeSetAdsEnabled(Profile.getLastUsedRegularProfile());
            }
        }).create();
        alertDialog.show();

        ImageView closeButton = alertDialog.findViewById(R.id.close_button);
        closeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                alertDialog.cancel();
            }
        });
    }

    public static void showExistingUserDialog(Context context) {
        AlertDialog alertDialog = new AlertDialog.Builder(context, R.style.BraveDialogTheme)
        .setView(R.layout.brave_ads_existing_user_dialog_layout)
        .setPositiveButton(R.string.brave_ads_offer_positive, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                // Enable ads
                neverShowOnboardingDialogAgain();

                BraveAdsNativeHelper.nativeSetAdsEnabled(Profile.getLastUsedRegularProfile());
            }
        }).create();
        alertDialog.show();

        ImageView closeButton = alertDialog.findViewById(R.id.close_button);
        closeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                alertDialog.cancel();
            }
        });
    }
    */
}
