/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

export const root = styled.div`
  display: flex;
  flex-direction: column;
  height: 100%;
  width: 364px;
  overflow: hidden;
`

export const loading = styled.div``

export const header = styled.div`
  text-align: center;
  padding-top: 27px;

  font-size: 16px;
  font-weight: 600;
  line-height: 24px;
`

export const tipKind = styled.div`
  padding: 22px 32px 0;
`

export const monthlyIndicator = styled.div`
  text-align: center;
  padding-bottom: 4px;
  margin-top: -20px;

  font-size: 12px;
  line-height: 16px;
  color: #979797;
`

export const monthlyIndicatorStar = styled.span`
  color: #4C54D2;
`

export const main = styled.div`
  width: 200%;
  flex: 1 0 auto;
  margin-top: 17px;
  margin-left: 0;
  display: flex;
  transition: margin-left .3s ease;

  &[data-active-form='2'] {
    margin-left: -100%;
  }

  > * {
    width: 100%;
  }
`