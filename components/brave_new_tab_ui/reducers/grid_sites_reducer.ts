// Copyright (c) 2019 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

// Redux API
import { Reducer } from 'redux'

// Types
import { types } from '../constants/grid_sites_types'

// API
import * as gridSitesState from '../state/gridSitesState'
import * as storage from '../storage/grid_sites_storage'
import {
  deleteMostVisitedTile,
  reorderMostVisitedTile
} from '../api/topSites'

// Utils
import arrayMove from 'array-move'

const initialState = storage.load()

export const gridSitesReducer: Reducer<NewTab.GridSitesState | undefined> = (
  state: NewTab.GridSitesState | undefined,
  action
) => {
  if (state === undefined) {
    state = initialState
  }

  const payload = action.payload
  const startingState = state

  switch (action.type) {
    case types.GRID_SITES_DATA_UPDATED: {
      state = gridSitesState.tilesUpdated(state, payload.gridSites)
      break
    }

    case types.GRID_SITES_REMOVE: {
      const { url } = payload
      deleteMostVisitedTile(url)
      state = gridSitesState.showTilesRemovedNotice(state, true)
      break
    }

    case types.GRID_SITES_REORDER: {
      // Change the order in Chromium
      const { gridSites, old_pos, new_pos } = payload
      reorderMostVisitedTile(gridSites[old_pos].url, new_pos)
      // Change the order that user sees. Chromium will overwrite this
      // when `MostVisitedInfoChanged` is called- but changing BEFORE that
      // avoids a flicker for the user where (for a second or so), tiles would
      // have the wrong order.
      const reorderedGridSites =
          arrayMove(gridSites, old_pos, new_pos) as NewTab.Site[]
      state = gridSitesState.tilesUpdated(state, reorderedGridSites)
      break
    }

    case types.GRID_SITES_SHOW_SITE_REMOVED_NOTIFICATION: {
      state = gridSitesState.showTilesRemovedNotice(state, payload.shouldShow)
      break
    }
  }

  if (JSON.stringify(state) !== JSON.stringify(startingState)) {
    storage.debouncedSave(state)
  }

  return state
}

export default gridSitesReducer
