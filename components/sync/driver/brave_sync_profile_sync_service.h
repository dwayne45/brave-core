/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_SYNC_DRIVER_BRAVE_SYNC_PROFILE_SYNC_SERVICE_H_
#define BRAVE_COMPONENTS_SYNC_DRIVER_BRAVE_SYNC_PROFILE_SYNC_SERVICE_H_

#include <string>

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "brave/components/brave_sync/brave_sync_prefs.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/sync/driver/profile_sync_service.h"

namespace syncer {

class BraveSyncAuthManager;
class DeviceInfoSyncService;

class BraveProfileSyncService : public ProfileSyncService {
 public:
  explicit BraveProfileSyncService(InitParams init_params);
  ~BraveProfileSyncService() override;

  // SyncService implementation
  bool IsSetupInProgress() const override;

  std::string GetOrCreateSyncCode();
  bool SetSyncCode(const std::string& sync_code);
  void ResetSync(DeviceInfoSyncService* device_info_service,
                 base::OnceClosure cb);

 private:
  BraveSyncAuthManager* GetBraveSyncAuthManager();

  void OnBraveSyncPrefsChanged(const std::string& path);

  void OnSelfDeleted(base::OnceClosure cb);

  brave_sync::Prefs brave_sync_prefs_;

  PrefChangeRegistrar brave_sync_prefs_change_registrar_;

  base::WeakPtrFactory<BraveProfileSyncService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(BraveProfileSyncService);
};
}  // namespace syncer

#endif  // BRAVE_COMPONENTS_SYNC_DRIVER_BRAVE_SYNC_PROFILE_SYNC_SERVICE_H_
