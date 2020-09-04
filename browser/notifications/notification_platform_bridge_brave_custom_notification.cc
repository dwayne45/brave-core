/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/notifications/notification_platform_bridge_brave_custom_notification.h"

#include "base/bind.h"
#include "base/callback.h"
#include "base/memory/scoped_refptr.h"
#include "base/run_loop.h"
#include "base/task/post_task.h"
#include "brave/components/brave_ads/browser/ads_notification_handler.h"
#include "brave/components/brave_ads/browser/ads_service.h"
#include "brave/components/brave_ads/browser/ads_service_factory.h"
#include "brave/components/brave_ads/browser/ads_service_impl.h"
#include "brave/ui/brave_custom_notification/message_popup_view.h"
#include "brave/ui/brave_custom_notification/public/cpp/notification.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/notifications/notification_ui_manager.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"

#if defined(OS_ANDROID)
#include "base/android/jni_android.h"
#include "base/android/jni_array.h"
#include "base/android/jni_string.h"
using base::android::AttachCurrentThread;
using base::android::ConvertJavaStringToUTF8;
using base::android::ConvertJavaStringToUTF16;
using base::android::ConvertUTF16ToJavaString;
using base::android::ConvertUTF8ToJavaString;
using base::android::JavaParamRef;
using base::android::ScopedJavaLocalRef;
#endif

namespace {

class PassThroughDelegate
    : public brave_custom_notification::NotificationDelegate {
 public:
  PassThroughDelegate(
      Profile* profile,
      const brave_custom_notification::Notification& notification)
      : profile_(profile),
        notification_(notification) {}

  void Close(bool by_user) override {
    std::unique_ptr<brave_ads::AdsNotificationHandler> handler =
        std::make_unique<brave_ads::AdsNotificationHandler>(
            static_cast<content::BrowserContext*>(profile_));
    handler->SetAdsService(static_cast<brave_ads::AdsServiceImpl*>(brave_ads::AdsServiceFactory::GetForProfile(profile_)));
    handler->OnClose(profile_, notification_.origin_url(), notification_.id(), by_user, base::OnceClosure());
  }

  void Click(const base::Optional<int>& button_index,
             const base::Optional<base::string16>& reply) override {
    std::unique_ptr<brave_ads::AdsNotificationHandler> handler = std::make_unique<brave_ads::AdsNotificationHandler>(static_cast<content::BrowserContext*>(profile_));
    handler->SetAdsService(static_cast<brave_ads::AdsServiceImpl*>(brave_ads::AdsServiceFactory::GetForProfile(profile_)));
    handler->OnClick(profile_, notification_.origin_url(), notification_.id(), button_index, reply, base::OnceClosure());
  }

 protected:
  ~PassThroughDelegate() override = default;

 private:
  Profile* profile_;
  brave_custom_notification::Notification notification_;

  DISALLOW_COPY_AND_ASSIGN(PassThroughDelegate);
};

}  // namespace

NotificationPlatformBridgeBraveCustomNotification::
    NotificationPlatformBridgeBraveCustomNotification(Profile* profile)
    : profile_(profile) {}

NotificationPlatformBridgeBraveCustomNotification::
    ~NotificationPlatformBridgeBraveCustomNotification() = default;

void NotificationPlatformBridgeBraveCustomNotification::Display(
    Profile* profile,
    brave_custom_notification::Notification& notification) {
  DCHECK_EQ(profile, profile_);

  // If there's no delegate, replace it with a PassThroughDelegate so clicks
  // go back to the appropriate handler.
  notification.set_delegate(base::WrapRefCounted(
      new PassThroughDelegate(profile_, notification)));

#if defined(OS_WIN) || defined(OS_MACOSX) || defined(OS_LINUX)
  brave_custom_notification::MessagePopupView::Show(notification);
#elif defined(OS_ANDROID)
  ShowAndroidAdsCustomNotification(profile, notification);
#endif

  std::unique_ptr<brave_ads::AdsNotificationHandler> handler = std::make_unique<brave_ads::AdsNotificationHandler>(static_cast<content::BrowserContext*>(profile));
  handler->OnShow(profile_, notification.id());
}

void NotificationPlatformBridgeBraveCustomNotification::ShowAndroidAdsCustomNotification(
    Profile* profile,
    brave_custom_notification::Notification& notification) {
  GURL origin_url(notification.origin_url().GetOrigin());

  JNIEnv* env = AttachCurrentThread();

  base::android::ScopedJavaLocalRef<jstring> j_scope_url =
      ConvertUTF8ToJavaString(env, origin_url.spec());

  base::android::ScopedJavaLocalRef<jstring> j_notification_id =
      ConvertUTF8ToJavaString(env, notification.id());
  base::android::ScopedJavaLocalRef<jstring> j_origin =
      ConvertUTF8ToJavaString(env, origin_url.spec());
  base::android::ScopedJavaLocalRef<jstring> title =
      ConvertUTF16ToJavaString(env, notification.title());
  base::android::ScopedJavaLocalRef<jstring> body =
      ConvertUTF16ToJavaString(env, notification.message());

  ScopedJavaLocalRef<jintArray> vibration_pattern =
      base::android::ToJavaIntArray(env, notification.vibration_pattern());

  ScopedJavaLocalRef<jstring> j_profile_id =
      ConvertUTF8ToJavaString(env, "");

  /*
  Java_NotificationPlatformBridge_displayNotification(
      env, java_object_, j_notification_id, j_notification_type, j_origin,
      j_scope_url, j_profile_id, profile->IsOffTheRecord(), title, body, image,
      notification_icon, badge, vibration_pattern,
      notification.timestamp().ToJavaTime(), notification.renotify(),
      notification.silent(), actions);
      */
}

void NotificationPlatformBridgeBraveCustomNotification::CloseAndroidAdsCustomNotification(
    Profile* profile,
    const std::string& notification_id) {
  JNIEnv* env = AttachCurrentThread();

  ScopedJavaLocalRef<jstring> j_notification_id =
      ConvertUTF8ToJavaString(env, notification_id);

  /*
  Java_NotificationPlatformBridge_closeNotification(
      env, java_object_, j_notification_id, j_scope_url,
      has_queried_webapk_package, j_webapk_package);
      */
}

void NotificationPlatformBridgeBraveCustomNotification::Close(
    Profile* profile,
    const std::string& notification_id) {
#if defined(OS_WIN) || defined(OS_MACOSX) || defined(OS_LINUX)
  brave_custom_notification::MessagePopupView::ClosePopup();
#elif defined(OS_ANDROID)
  NotificationPlatformBridgeBraveCustomNotification::CloseAndroidAdsCustomNotification(profile, notification_id);
#endif
}

