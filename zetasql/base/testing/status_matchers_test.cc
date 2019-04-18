//
// Copyright 2018 ZetaSQL Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "zetasql/base/testing/status_matchers.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "zetasql/base/status.h"
#include "zetasql/base/status_macros.h"
#include "zetasql/base/statusor.h"

namespace {

using testing::_;
using testing::Not;
using zetasql_base::OkStatus;
using zetasql_base::testing::IsOkAndHolds;
using zetasql_base::testing::StatusIs;

zetasql_base::Status AbortedStatus() {
  return zetasql_base::Status(zetasql_base::StatusCode::kAborted, "aborted");
}

zetasql_base::StatusOr<int> OkStatusOr(int n) { return n; }

zetasql_base::StatusOr<int> AbortedStatusOr() { return AbortedStatus(); }

TEST(StatusMatcher, Macros) {
  ZETASQL_EXPECT_OK(OkStatus());
  ZETASQL_ASSERT_OK(OkStatus());
  ZETASQL_ASSERT_OK_AND_ASSIGN(int value, OkStatusOr(15));
  EXPECT_EQ(value, 15);
}

TEST(StatusMatcher, IsOkAndHolds) {
  EXPECT_THAT(OkStatusOr(15), IsOkAndHolds(15));
  EXPECT_THAT(OkStatusOr(15), Not(IsOkAndHolds(0)));
  EXPECT_THAT(AbortedStatusOr(), Not(IsOkAndHolds(0)));

  // Weird usage, but should work
  EXPECT_THAT(OkStatusOr(15), IsOkAndHolds(_));
}

TEST(StatusMatcher, StatusIs) {
  EXPECT_THAT(AbortedStatus(),
              StatusIs(zetasql_base::StatusCode::kAborted, "aborted"));
  EXPECT_THAT(AbortedStatus(), StatusIs(zetasql_base::StatusCode::kAborted, _));
  EXPECT_THAT(AbortedStatus(), StatusIs(_, "aborted"));
  EXPECT_THAT(AbortedStatus(), StatusIs(_, _));

  EXPECT_THAT(AbortedStatus(), StatusIs(zetasql_base::StatusCode::kAborted));
  EXPECT_THAT(AbortedStatus(), StatusIs(Not(zetasql_base::StatusCode::kOk)));
  EXPECT_THAT(AbortedStatus(), StatusIs(_));

  EXPECT_THAT(AbortedStatusOr(),
              StatusIs(zetasql_base::StatusCode::kAborted, "aborted"));
  EXPECT_THAT(AbortedStatusOr(),
              StatusIs(zetasql_base::StatusCode::kAborted, _));
  EXPECT_THAT(AbortedStatusOr(), StatusIs(_, "aborted"));
  EXPECT_THAT(AbortedStatusOr(), StatusIs(_, _));

  EXPECT_THAT(AbortedStatusOr(), StatusIs(zetasql_base::StatusCode::kAborted));
  EXPECT_THAT(AbortedStatusOr(), StatusIs(Not(zetasql_base::StatusCode::kOk)));
  EXPECT_THAT(AbortedStatusOr(), StatusIs(_));

  // Weird usages, but should work.
  EXPECT_THAT(OkStatusOr(15), StatusIs(zetasql_base::StatusCode::kOk, ""));
  EXPECT_THAT(OkStatusOr(15), StatusIs(zetasql_base::StatusCode::kOk, _));
  EXPECT_THAT(OkStatusOr(15), StatusIs(_, ""));
  EXPECT_THAT(OkStatusOr(15), StatusIs(_, _));

  EXPECT_THAT(OkStatusOr(15), StatusIs(zetasql_base::StatusCode::kOk));
  EXPECT_THAT(OkStatusOr(15),
              StatusIs(Not(zetasql_base::StatusCode::kAborted), _));
  EXPECT_THAT(OkStatusOr(15), StatusIs(_));

  EXPECT_THAT(OkStatus(), StatusIs(zetasql_base::StatusCode::kOk, ""));
  EXPECT_THAT(OkStatus(), StatusIs(zetasql_base::StatusCode::kOk, _));
  EXPECT_THAT(OkStatus(), StatusIs(_, ""));
  EXPECT_THAT(OkStatus(), StatusIs(_, _));

  EXPECT_THAT(OkStatus(), StatusIs(zetasql_base::StatusCode::kOk));
  EXPECT_THAT(OkStatus(), StatusIs(Not(zetasql_base::StatusCode::kAborted), _));
  EXPECT_THAT(OkStatus(), StatusIs(_));
}

}  // namespace
