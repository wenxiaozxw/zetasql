//
// Copyright 2019 ZetaSQL Authors
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

#include "zetasql/parser/parse_tree.h"

#include <algorithm>
#include <functional>
#include <queue>

#include "zetasql/base/logging.h"
#include "zetasql/parser/ast_node_kind.h"
#include "zetasql/parser/parse_tree_errors.h"
#include "zetasql/parser/parse_tree_visitor.h"
// This is not a header -- it is a generated part of this source file.
#include "zetasql/parser/parse_tree_accept_methods.inc"  
#include "zetasql/public/strings.h"
#include "absl/base/attributes.h"
#include "absl/container/flat_hash_map.h"
#include "absl/strings/match.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "zetasql/base/map_util.h"
#include "zetasql/base/ret_check.h"
#include "zetasql/base/status_macros.h"

namespace zetasql {

// Creates a map of ASTNodeKind to a std::string representation of the node type's
// name. Access this map through GetNodeNamesMap().
static absl::flat_hash_map<ASTNodeKind, std::string> CreateNodeNamesMap() {
  absl::flat_hash_map<ASTNodeKind, std::string> map;
  map[AST_FAKE] = "Fake";  // For testing purposes only.
  map[AST_ABORT_BATCH_STATEMENT] = "AbortBatchStatement";
  map[AST_ADD_CONSTRAINT_ACTION] = "AddConstraintAction";
  map[AST_ALIAS] = "Alias";
  map[AST_ALTER_ACTION_LIST] = "AlterActionList";
  map[AST_ALTER_CONSTRAINT_ENFORCEMENT_ACTION] =
      "AlterConstraintEnforcementAction";
  map[AST_ALTER_CONSTRAINT_SET_OPTIONS_ACTION] =
      "ASTAlterConstraintSetOptionsAction";
  map[AST_ALTER_ROW_POLICY_STATEMENT] = "AlterRowPolicyStatement";
  map[AST_ALTER_TABLE_STATEMENT] = "AlterTableStatement";
  map[AST_ALTER_VIEW_STATEMENT] = "AlterViewStatement";
  map[AST_ANALYTIC_FUNCTION_CALL] = "AnalyticFunctionCall";
  map[AST_AND_EXPR] = "AndExpr";
  map[AST_ARRAY_COLUMN_SCHEMA] = "ArrayColumnSchema";
  map[AST_ARRAY_CONSTRUCTOR] = "ArrayConstructor";
  map[AST_ARRAY_ELEMENT] = "ArrayElement";
  map[AST_ARRAY_TYPE] = "ArrayType";
  map[AST_ASSERT_STATEMENT] = "AssertStatement";
  map[AST_ASSERT_ROWS_MODIFIED] = "AssertRowsModified";
  map[AST_ASSIGNMENT_FROM_STRUCT] = "AssignmentFromStruct";
  map[AST_BEGIN_STATEMENT] = "BeginStatement";
  map[AST_BETWEEN_EXPRESSION] = "BetweenExpression";
  map[AST_BINARY_EXPRESSION] = "BinaryExpression";
  map[AST_BITWISE_SHIFT_EXPRESSION] = "BitwiseShiftExpression";
  map[AST_BEGIN_END_BLOCK] = "BeginEndBlock";
  map[AST_BOOLEAN_LITERAL] = "BooleanLiteral";
  map[AST_BREAK_STATEMENT] = "Break";
  map[AST_BYTES_LITERAL] = "BytesLiteral";
  map[AST_CALL_STATEMENT] = "CallStatement";
  map[AST_CASE_NO_VALUE_EXPRESSION] = "CaseNoValueExpression";
  map[AST_CASE_VALUE_EXPRESSION] = "CaseValueExpression";
  map[AST_CAST_EXPRESSION] = "CastExpression";
  map[AST_CHECK_CONSTRAINT] = "CheckConstraint";
  map[AST_CLUSTER_BY] = "ClusterBy";
  map[AST_COLLATE] = "Collate";
  map[AST_COLUMN_DEFINITION] = "ColumnDefinition";
  map[AST_COLUMN_ATTRIBUTE_LIST] = "ColumnAttributeList";
  map[AST_COLUMN_LIST] = "ColumnList";
  map[AST_COMMIT_STATEMENT] = "CommitStatement";
  map[AST_CONTINUE_STATEMENT] = "Continue";
  map[AST_CREATE_CONSTANT_STATEMENT] = "CreateConstantStatement";
  map[AST_CREATE_DATABASE_STATEMENT] = "CreateDatabaseStatement";
  map[AST_CREATE_EXTERNAL_TABLE_STATEMENT] = "CreateExternalTableStatement";
  map[AST_CREATE_FUNCTION_STATEMENT] = "CreateFunctionStatement";
  map[AST_CREATE_INDEX_STATEMENT] = "CreateIndexStatement";
  map[AST_CREATE_PROCEDURE_STATEMENT] = "CreateProcedureStatement";
  map[AST_CREATE_MODEL_STATEMENT] = "CreateModelStatement";
  map[AST_CREATE_ROW_POLICY_STATEMENT] = "CreateRowPolicyStatement";
  map[AST_CREATE_TABLE_FUNCTION_STATEMENT] = "CreateTableFunctionStatement";
  map[AST_CREATE_TABLE_STATEMENT] = "CreateTableStatement";
  map[AST_CREATE_VIEW_STATEMENT] = "CreateViewStatement";
  map[AST_CREATE_MATERIALIZED_VIEW_STATEMENT] =
      "CreateMaterializedViewStatement";
  map[AST_DATE_OR_TIME_LITERAL] = "DateOrTimeLiteral";
  map[AST_DEFAULT_LITERAL] = "DefaultLiteral";
  map[AST_DEFINE_TABLE_STATEMENT] = "DefineTableStatement";
  map[AST_DELETE_STATEMENT] = "DeleteStatement";
  map[AST_DESCRIBE_STATEMENT] = "DescribeStatement";
  map[AST_DOT_GENERALIZED_FIELD] = "DotGeneralizedField";
  map[AST_DOT_IDENTIFIER] = "DotIdentifier";
  map[AST_DOT_STAR_WITH_MODIFIERS] = "DotStarWithModifiers";
  map[AST_DOT_STAR] = "DotStar";
  map[AST_DROP_ALL_ROW_POLICIES_STATEMENT] = "DropAllRowPoliciesStatement";
  map[AST_DROP_CONSTRAINT_ACTION] = "DropConstraintAction";
  map[AST_DROP_FUNCTION_STATEMENT] = "DropFunctionStatement";
  map[AST_DROP_ROW_POLICY_STATEMENT] = "DropRowPolicyStatement";
  map[AST_DROP_STATEMENT] = "DropStatement";
  map[AST_DROP_MATERIALIZED_VIEW_STATEMENT] = "DropMaterializedViewStatement";
  map[AST_EXPLAIN_STATEMENT] = "ExplainStatement";
  map[AST_EXPORT_DATA_STATEMENT] = "ExportDataStatement";
  map[AST_EXPRESSION_SUBQUERY] = "ExpressionSubquery";
  map[AST_EXTRACT_EXPRESSION] = "ExtractExpression";
  map[AST_FLOAT_LITERAL] = "FloatLiteral";
  map[AST_FOREIGN_KEY] = "ForeignKey";
  map[AST_FOREIGN_KEY_ACTIONS] = "ForeignKeyActions";
  map[AST_FOREIGN_KEY_COLUMN_ATTRIBUTE] = "ForeignKeyColumnAttribute";
  map[AST_FOREIGN_KEY_REFERENCE] = "ForeignKeyReference";
  map[AST_FOR_SYSTEM_TIME] = "ForSystemTime";
  map[AST_FROM_CLAUSE] = "FromClause";
  map[AST_FUNCTION_CALL] = "FunctionCall";
  map[AST_FUNCTION_DECLARATION] = "FunctionDeclaration";
  map[AST_FUNCTION_PARAMETERS] = "FunctionParameters";
  map[AST_FUNCTION_PARAMETER] = "FunctionParameter";
  map[AST_GENERATED_COLUMN_INFO] = "GeneratedColumnInfo";
  map[AST_GRANTEE_LIST] = "GranteeList";
  map[AST_GRANT_STATEMENT] = "GrantStatement";
  map[AST_GROUP_BY] = "GroupBy";
  map[AST_GROUPING_ITEM] = "GroupingItem";
  map[AST_HAVING_MODIFIER] = "HavingModifier";
  map[AST_HAVING] = "Having";
  map[AST_HIDDEN_COLUMN_ATTRIBUTE] = "HiddenColumnAttribute";
  map[AST_HINTED_STATEMENT] = "HintedStatement";
  map[AST_HINT_ENTRY] = "HintEntry";
  map[AST_HINT] = "Hint";
  map[AST_IDENTIFIER] = "Identifier";
  map[AST_IDENTIFIER_LIST] = "IdentifierList";
  map[AST_IF_STATEMENT] = "IfStatement";
  map[AST_IMPORT_STATEMENT] = "ImportStatement";
  map[AST_IN_EXPRESSION] = "InExpression";
  map[AST_IN_LIST] = "InList";
  map[AST_INDEX_ITEM_LIST] = "IndexItemList";
  map[AST_INDEX_STORING_EXPRESSION_LIST] = "IndexStoringExpressionList";
  map[AST_INDEX_UNNEST_EXPRESSION_LIST] = "IndexUnnestExpressionList";
  map[AST_INFERRED_TYPE_COLUMN_SCHEMA] = "InferredTypeColumnSchema";
  map[AST_INSERT_STATEMENT] = "InsertStatement";
  map[AST_INSERT_VALUES_ROW_LIST] = "InsertValuesRowList";
  map[AST_INSERT_VALUES_ROW] = "InsertValuesRow";
  map[AST_INTERVAL_EXPR] = "IntervalExpr";
  map[AST_INTO_ALIAS] = "IntoAlias";
  map[AST_INT_LITERAL] = "IntLiteral";
  map[AST_JOIN] = "Join";
  map[AST_LIMIT_OFFSET] = "LimitOffset";
  map[AST_MERGE_ACTION] = "MergeAction";
  map[AST_MERGE_STATEMENT] = "MergeStatement";
  map[AST_MERGE_WHEN_CLAUSE] = "MergeWhenClause";
  map[AST_MERGE_WHEN_CLAUSE_LIST] = "MergeWhenClauseList";
  map[AST_MODEL_CLAUSE] = "ModelClause";
  map[AST_MODULE_STATEMENT] = "ModuleStatement";
  map[AST_NAMED_ARGUMENT] = "NamedArgument";
  map[AST_NEW_CONSTRUCTOR_ARG] = "NewConstructorArg";
  map[AST_NEW_CONSTRUCTOR] = "NewConstructor";
  map[AST_NOT_NULL_COLUMN_ATTRIBUTE] = "NotNullColumnAttribute";
  map[AST_NULL_LITERAL] = "NullLiteral";
  map[AST_NUMERIC_LITERAL] = "NumericLiteral";
  map[AST_ON_CLAUSE] = "OnClause";
  map[AST_OPTIONS_ENTRY] = "OptionsEntry";
  map[AST_OPTIONS_LIST] = "OptionsList";
  map[AST_ORDER_BY] = "OrderBy";
  map[AST_ORDERING_EXPRESSION] = "OrderingExpression";
  map[AST_OR_EXPR] = "OrExpr";
  map[AST_PARAMETER_EXPR] = "ParameterExpr";
  map[AST_PARENTHESIZED_JOIN] = "ParenthesizedJoin";
  map[AST_PARTITION_BY] = "PartitionBy";
  map[AST_PATH_EXPRESSION] = "PathExpression";
  map[AST_PRIMARY_KEY] = "PrimaryKey";
  map[AST_PRIMARY_KEY_COLUMN_ATTRIBUTE] = "PrimaryKeyColumnAttribute";
  map[AST_PRIVILEGES] = "Privileges";
  map[AST_PRIVILEGE] = "Privilege";
  map[AST_QUERY_STATEMENT] = "QueryStatement";
  map[AST_QUERY] = "Query";
  map[AST_RENAME_STATEMENT] = "RenameStatement";
  map[AST_REPEATABLE_CLAUSE] = "RepeatableClause";
  map[AST_REPLACE_FIELDS_EXPRESSION] = "ReplaceFieldsExpression";
  map[AST_REVOKE_STATEMENT] = "RevokeStatement";
  map[AST_ROLLBACK_STATEMENT] = "RollbackStatement";
  map[AST_ROLLUP] = "Rollup";
  map[AST_RUN_BATCH_STATEMENT] = "RunBatchStatement";
  map[AST_SAMPLE_CLAUSE] = "SampleClause";
  map[AST_SAMPLE_SIZE] = "SampleSize";
  map[AST_SAMPLE_SUFFIX] = "SampleSuffix";
  map[AST_SCRIPT] = "Script";
  map[AST_SELECT_AS] = "SelectAs";
  map[AST_SELECT_COLUMN] = "SelectColumn";
  map[AST_SELECT_LIST] = "SelectList";
  map[AST_SELECT] = "Select";
  map[AST_SET_OPERATION] = "SetOperation";
  map[AST_SET_OPTIONS_ACTION] = "SetOptionsOperation";
  map[AST_SET_TRANSACTION_STATEMENT] = "SetTransaction";
  map[AST_SINGLE_ASSIGNMENT] = "SingleAssignment";
  map[AST_SHOW_STATEMENT] = "ShowStatement";
  map[AST_SIMPLE_COLUMN_SCHEMA] = "SimpleColumnSchema";
  map[AST_SIMPLE_TYPE] = "SimpleType";
  map[AST_SQL_FUNCTION_BODY] = "SqlFunctionBody";
  map[AST_STAR_EXCEPT_LIST] = "StarExceptList";
  map[AST_STAR_MODIFIERS] = "StarModifiers";
  map[AST_STAR_REPLACE_ITEM] = "StarReplaceItem";
  map[AST_STAR_WITH_MODIFIERS] = "StarWithModifiers";
  map[AST_STAR] = "Star";
  map[AST_START_BATCH_STATEMENT] = "StartBatchStatement";
  map[AST_STATEMENT_LIST] = "StatementList";
  map[AST_STRING_LITERAL] = "StringLiteral";
  map[AST_STRUCT_COLUMN_FIELD] = "StructColumnField";
  map[AST_STRUCT_COLUMN_SCHEMA] = "StructColumnSchema";
  map[AST_STRUCT_CONSTRUCTOR_ARG] = "StructConstructorArg";
  map[AST_STRUCT_CONSTRUCTOR_WITH_KEYWORD] = "StructConstructorWithKeyword";
  map[AST_STRUCT_CONSTRUCTOR_WITH_PARENS] = "StructConstructorWithParens";
  map[AST_STRUCT_FIELD] = "StructField";
  map[AST_STRUCT_TYPE] = "StructType";
  map[AST_TABLE_CLAUSE] = "TableClause";
  map[AST_TABLE_ELEMENT_LIST] = "TableElementList";
  map[AST_TABLE_PATH_EXPRESSION] = "TablePathExpression";
  map[AST_TABLE_SUBQUERY] = "TableSubquery";
  map[AST_TEMPLATED_PARAMETER_TYPE] = "TemplatedParameterType";
  map[AST_TRANSACTION_ISOLATION_LEVEL] = "TransactionIsolationLevel";
  map[AST_TRANSACTION_MODE_LIST] = "TransactionModeList";
  map[AST_TRANSACTION_READ_WRITE_MODE] = "TransactionReadWriteMode";
  map[AST_TRANSFORM_CLAUSE] = "TransformClause";
  map[AST_TVF_ARGUMENT] = "TVFArgument";
  map[AST_TVF_SCHEMA_COLUMN] = "TVFSchemaColumn";
  map[AST_TVF_SCHEMA] = "TVFSchema";
  map[AST_TVF] = "TVF";
  map[AST_UNARY_EXPRESSION] = "UnaryExpression";
  map[AST_UNNEST_EXPRESSION] = "UnnestExpression";
  map[AST_UNNEST_EXPRESSION_WITH_OPT_ALIAS_AND_OFFSET] =
      "UnnestExpressionWithOptAliasAndOffset";
  map[AST_UPDATE_ITEM_LIST] = "UpdateItemList";
  map[AST_UPDATE_ITEM] = "UpdateItem";
  map[AST_UPDATE_SET_VALUE] = "UpdateSetValue";
  map[AST_UPDATE_STATEMENT] = "UpdateStatement";
  map[AST_USING_CLAUSE] = "UsingClause";
  map[AST_VARIABLE_DECLARATION] = "VariableDeclaration";
  map[AST_WHERE_CLAUSE] = "WhereClause";
  map[AST_WHILE_STATEMENT] = "While";
  map[AST_WINDOW_CLAUSE] = "WindowClause";
  map[AST_WINDOW_DEFINITION] = "WindowDefinition";
  map[AST_WINDOW_FRAME_EXPR] = "WindowFrameExpr";
  map[AST_WINDOW_FRAME] = "WindowFrame";
  map[AST_WINDOW_SPECIFICATION] = "WindowSpecification";
  map[AST_WITH_CLAUSE_ENTRY] = "WithClauseEntry";
  map[AST_WITH_CLAUSE] = "WithClause";
  map[AST_WITH_OFFSET] = "WithOffset";
  map[AST_WITH_WEIGHT] = "WithWeight";
  for (int kind = kFirstASTNodeKind; kind <= kLastASTNodeKind;
       ++kind) {
    DCHECK(zetasql_base::ContainsKey(map, static_cast<ASTNodeKind>(kind)))
        << "kind=" << kind;
  }
  return map;
}

// Returns a map of ASTNodeKind to a std::string representation of the node type's
// name.
static const absl::flat_hash_map<ASTNodeKind, std::string>& GetNodeNamesMap() {
  static const absl::flat_hash_map<ASTNodeKind, std::string>& map =
      *new auto(CreateNodeNamesMap());
  return map;
}

ASTNode::~ASTNode() {}

void ASTNode::AddChild(ASTNode* child) {
  DCHECK(child != nullptr);
  children_.push_back(child);
  child->set_parent(this);
}

void ASTNode::AddChildren(absl::Span<ASTNode* const> children) {
  for (ASTNode* child : children) {
    if (child != nullptr) {
      children_.push_back(child);
      child->set_parent(this);
    }
  }
}

void ASTNode::Accept(ParseTreeVisitor* visitor, void* data) const {
  visitor->visit(this, data);
}

void ASTNode::ChildrenAccept(ParseTreeVisitor* visitor, void* data) const {
  for (int i = 0; i < children_.size(); ++i) {
    children_[i]->Accept(visitor, data);
  }
}

std::string ASTNode::NodeKindToString(ASTNodeKind node_kind) {
  // Subtle: we must ensure that default_value outlives the FindWithDefault
  // call.
  const std::string default_value = "<UNKNOWN NODE KIND>";
  return zetasql_base::FindWithDefault(GetNodeNamesMap(), node_kind, default_value);
}

std::string ASTNode::GetNodeKindString() const {
  return NodeKindToString(node_kind());
}

std::string ASTNode::SingleNodeDebugString() const {
  return NodeKindToString(node_kind());
}

// This function is not inlined, to minimize the stack usage of Dump().
ABSL_ATTRIBUTE_NOINLINE bool ASTNode::Dumper::DumpNode() {
  out_->append(current_depth_ * 2, ' ');
  absl::StrAppend(out_, node_->SingleNodeDebugString(), " [",
                  node_->GetParseLocationRange().GetString(), "]", separator_);
  if (current_depth_ >= max_depth_) {
    out_->append(current_depth_ * 2, ' ');
    absl::StrAppend(out_, "  Subtree skipped (reached max depth ", max_depth_,
                    ")", separator_);
    return false;
  }
  return true;
}

// This function is recursive. To minimize the risk of stack overflow, it
// has minimal arguments and local variables, and it does not call any inline
// function with large local variables. In particular, it does not call
// StrCat or StrAppend or any inline function that calls these 2 functions,
// which takes 48 bytes (= sizeof(AlphaNum)) per argument (the compiler does
// not let the temporary objects share memory). The current implementation
// has 48 bytes frame size.
void ASTNode::Dumper::Dump() {
  if (!DumpNode()) {
    return;
  }
  ++current_depth_;
  const auto& children = node_->children_;
  for (ASTNode* n : children) {
    if (n != nullptr) {
      node_ = n;
      Dump();
    }
  }
  --current_depth_;
}

std::string ASTNode::DebugString(int max_depth) const {
  std::string out;
  Dumper(this, "\n", max_depth, &out).Dump();
  return out;
}

std::string ASTNode::GetLocationString() const {
  return parse_location_range_.GetString();
}

// NOTE: An equivalent method on ResolvedNodes exists in
// ../resolved_ast/resolved_node.cc.
void ASTNode::GetDescendantsWithKindsImpl(
    const std::set<int>& node_kinds,
    std::vector<const ASTNode*>* found_nodes,
    bool continue_traversal) const {
  found_nodes->clear();

  // Use non-recursive traversal to avoid stack issues.
  std::queue<const ASTNode*> node_queue;
  node_queue.push(this);

  while (!node_queue.empty()) {
    const ASTNode* node = node_queue.front();
    node_queue.pop();

    if (zetasql_base::ContainsKey(node_kinds, node->node_kind())) {
      // Emit this node.
      found_nodes->push_back(node);

      if (!continue_traversal) {
        continue;
      }
    }
    // Queue its children for traversal.
    for (int i = 0; i < node->num_children(); ++i) {
      node_queue.push(node->child(i));
    }
  }
}

std::pair<std::string, std::string> ASTSetOperation::GetSQLForOperationPair() const {
  if (op_type_ == NOT_SET) {
    return std::make_pair("<UNKNOWN SET OPERATOR>", "");
  }
  return std::make_pair(
      op_type_ == UNION ? "UNION" : op_type_ == EXCEPT ? "EXCEPT" : "INTERSECT",
      distinct_ ? "DISTINCT" : "ALL");
}

std::string ASTSetOperation::GetSQLForOperation() const {
  auto pair = GetSQLForOperationPair();
  return absl::StrCat(pair.first, " ", pair.second);
}

std::string ASTSetOperation::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                      GetSQLForOperation(), ")");
}

std::string ASTSelect::SingleNodeDebugString() const {
  std::vector<std::string> select_attrs;
  if (distinct()) {
    select_attrs.push_back("distinct=true");
  }
  if (select_attrs.empty()) {
    return ASTNode::SingleNodeDebugString();
  }
  return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                      absl::StrJoin(select_attrs, ", "), ")");
}

std::string ASTSelectAs::SingleNodeDebugString() const {
  if (as_mode_ == TYPE_NAME) {
    return ASTNode::SingleNodeDebugString();
  } else {
    return absl::StrCat(ASTNode::SingleNodeDebugString(),
                        "(as_mode=", as_mode() == VALUE ? "VALUE" : "STRUCT",
                        ")");
  }
}

std::string ASTAlias::GetAsString() const {
  return identifier()->GetAsString();
}

std::string ASTIntoAlias::GetAsString() const {
  return identifier()->GetAsString();
}

const ASTNode* ASTTableExpression::alias_location() const {
  const ASTAlias* ast_alias = alias();
  if (ast_alias != nullptr) return ast_alias->identifier();
  return this;
}

std::string ASTJoin::SingleNodeDebugString() const {
  std::vector<std::string> join_attrs;
  if (natural()) {
    join_attrs.push_back("NATURAL");
  }
  if (join_type() != DEFAULT_JOIN_TYPE) {
    // Show "Join(COMMA)" rather than "Join(,)" for comma join.
    join_attrs.push_back(join_type() == COMMA ? "COMMA" : GetSQLForJoinType());
  }
  if (join_hint() != NO_JOIN_HINT) {
    join_attrs.push_back(GetSQLForJoinHint());
  }

  if (join_attrs.empty()) {
    return ASTNode::SingleNodeDebugString();
  }

  return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                      absl::StrJoin(join_attrs, ", "), ")");
}

std::string ASTJoin::GetSQLForJoinType() const {
  switch (join_type_) {
    case DEFAULT_JOIN_TYPE:
      return "";
    case COMMA:
      return ",";
    case CROSS:
      return "CROSS";
    case FULL:
      return "FULL";
    case INNER:
      return "INNER";
    case LEFT:
      return "LEFT";
    case RIGHT:
      return "RIGHT";
  }
}

std::string ASTJoin::GetSQLForJoinHint() const {
  switch (join_hint_) {
    case NO_JOIN_HINT:
      return "";
    case HASH:
      return "HASH";
    case LOOKUP:
      return "LOOKUP";
  }
}

std::string ASTOrderingExpression::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(),
                      descending() ? "(DESC)" : "(ASC)");
}

zetasql_base::Status
ASTGeneralizedPathExpression::VerifyIsPureGeneralizedPathExpression(
    const ASTExpression* path) {
  while (true) {
    switch (path->node_kind()) {
      case AST_PATH_EXPRESSION:
        return ::zetasql_base::OkStatus();
      case AST_DOT_GENERALIZED_FIELD:
        path = path->GetAs<ASTDotGeneralizedField>()->expr();
        break;
      case AST_DOT_IDENTIFIER:
        path = path->GetAs<ASTDotIdentifier>()->expr();
        break;
      case AST_ARRAY_ELEMENT:
        path = path->GetAs<ASTArrayElement>()->array();
        break;
      default:
        // This returns the rightmost error, but that's ok since this method is
        // just for debugging.
        return MakeSqlErrorAt(path) << "Expected pure generalized path "
                                    << "expression, but found node kind "
                                    << path->GetNodeKindString();
    }
  }
}

std::string ASTBinaryExpression::GetSQLForOperator() const {
  switch (op_) {
    case NOT_SET:
      return "<UNKNOWN OPERATOR>";
    case LIKE:
      return is_not_ ? "NOT LIKE" : "LIKE";
    case IS:
      return is_not_ ? "IS NOT" : "IS";
    case EQ:
      return "=";
    case NE:
      return "!=";
    case NE2:
      return "<>";
    case GT:
      return ">";
    case LT:
      return "<";
    case GE:
      return ">=";
    case LE:
      return "<=";
    case BITWISE_OR:
      return "|";
    case BITWISE_XOR:
      return "^";
    case BITWISE_AND:
      return "&";
    case PLUS:
      return "+";
    case MINUS:
      return "-";
    case MULTIPLY:
      return "*";
    case DIVIDE:
      return "/";
  }
}

std::string ASTBinaryExpression::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                      GetSQLForOperator(), ")");
}

std::string ASTBitwiseShiftExpression::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                      is_left_shift() ? "<<" : ">>", ")");
}

std::string ASTInExpression::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                      is_not_ ? "NOT " : "", "IN", ")");
}

std::string ASTBetweenExpression::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                      is_not_ ? "NOT " : "", "BETWEEN)");
}

std::string ASTUnaryExpression::SingleNodeDebugString() const {
  // Include the image and location information
  return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                      GetSQLForOperator(), ")");
}

std::string ASTUnaryExpression::GetSQLForOperator() const {
  switch (op_) {
    case NOT_SET:
      return "<UNKNOWN OPERATOR>";
    case NOT:
      return "NOT";
    case BITWISE_NOT:
      return "~";
    case MINUS:
      return "-";
    case PLUS:
      return "+";
  }
}

std::string ASTCastExpression::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(),
                      is_safe_cast_ ? "(return_null_on_error=true)" : "");
}

std::string ASTDropStatement::SingleNodeDebugString() const {
  const std::string out = absl::StrCat(ASTNode::SingleNodeDebugString(), " ",
                                  SchemaObjectKindToName(schema_object_kind()));
  if (!is_if_exists()) {
    return out;
  }
  return absl::StrCat(out, "(is_if_exists)");
}

std::string ASTDropFunctionStatement::SingleNodeDebugString() const {
  const std::string& node_name = ASTNode::SingleNodeDebugString();
  return (!is_if_exists()) ? node_name
                           : absl::StrCat(node_name, "(is_if_exists)");
}

std::string ASTDropRowPolicyStatement::SingleNodeDebugString() const {
  const std::string& node_name = ASTNode::SingleNodeDebugString();
  if (!is_if_exists()) {
    return node_name;
  }
  return absl::StrCat(node_name, "(is_if_exists)");
}

std::string ASTDropMaterializedViewStatement::SingleNodeDebugString() const {
  const std::string& node_name = ASTNode::SingleNodeDebugString();
  if (!is_if_exists()) {
    return node_name;
  }
  return absl::StrCat(node_name, "(is_if_exists)");
}

std::string ASTPathExpression::ToIdentifierPathString(size_t max_prefix_size) const {
  const int end = max_prefix_size == 0
                      ? names_.size()
                      : std::min(names_.size(), max_prefix_size);
  std::string ret;
  for (int i = 0; i < end; ++i) {
    if (i != 0) ret += ".";
    ret += ToIdentifierLiteral(names_[i]->GetAsString());
  }
  return ret;
}

std::vector<std::string> ASTPathExpression::ToIdentifierVector() const {
  std::vector<std::string> ret;
  ret.reserve(names_.size());
  for (const ASTIdentifier* name : names_) {
    ret.push_back(name->GetAsString());
  }
  return ret;
}

std::string ASTParameterExpr::SingleNodeDebugString() const {
  if (name() != nullptr) {
    return ASTNode::SingleNodeDebugString();
  } else {
    return absl::StrCat(ASTNode::SingleNodeDebugString(), "(", position_, ")");
  }
}

std::string ASTFunctionCall::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(),
                      distinct() ? "(distinct=true)" : "");
}

std::string ASTWindowFrame::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                      GetFrameUnitString(), ")");
}

// static
std::string ASTWindowFrame::FrameUnitToString(FrameUnit unit) {
  switch (unit) {
    case ROWS:
      return "ROWS";
    case RANGE:
      return "RANGE";
    default:
      LOG(DFATAL) << "Unknown analytic window frame unit: " << unit;
      return "";
  }
}

std::string ASTWindowFrame::GetFrameUnitString() const {
  return FrameUnitToString(frame_unit_);
}

std::string ASTWindowFrameExpr::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                      GetBoundaryTypeString(), ")");
}

// static
std::string ASTWindowFrameExpr::BoundaryTypeToString(BoundaryType type) {
  switch (type) {
    case UNBOUNDED_PRECEDING:
      return "UNBOUNDED PRECEDING";
    case OFFSET_PRECEDING:
      return "OFFSET PRECEDING";
    case CURRENT_ROW:
      return "CURRENT ROW";
    case OFFSET_FOLLOWING:
      return "OFFSET FOLLOWING";
    case UNBOUNDED_FOLLOWING:
      return "UNBOUNDED FOLLOWING";
    default:
      LOG(DFATAL) << "Unknown analytic window frame expression type:" << type;
      return "";
  }
}

std::string ASTWindowFrameExpr::GetBoundaryTypeString() const {
  return BoundaryTypeToString(boundary_type_);
}

std::string ASTExpressionSubquery::ModifierToString(Modifier modifier) {
  switch (modifier) {
    case ARRAY:
      return "ARRAY";
    case EXISTS:
      return "EXISTS";
    case NONE:
      return "";
  }
}

std::string ASTExpressionSubquery::SingleNodeDebugString() const {
  if (modifier_ == NONE) {
    return ASTNode::SingleNodeDebugString();
  } else {
    return absl::StrCat(ASTNode::SingleNodeDebugString(),
                        "(modifier=", ModifierToString(modifier_), ")");
  }
}

std::string ASTIdentifier::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                      ToIdentifierLiteral(id_string_), ")");
}

bool ASTIntLiteral::is_hex() const {
  if (absl::StartsWith(image(), "0x") || absl::StartsWith(image(), "0X")) {
    return true;
  }
  return false;
}

std::string ASTDateOrTimeLiteral::SingleNodeDebugString() const {
  return absl::StrCat("DateOrTimeLiteral(", TypeKind_Name(type_kind_), ")");
}

std::string ASTCreateStatement::SingleNodeDebugString() const {
  std::vector<std::string> modifiers;
  switch (scope_) {
    case ASTCreateStatement::PRIVATE:
      modifiers.push_back("is_private");
      break;
    case ASTCreateStatement::PUBLIC:
      modifiers.push_back("is_public");
      break;
    case ASTCreateStatement::TEMPORARY:
      modifiers.push_back("is_temp");
      break;
    case ASTCreateStatement::DEFAULT_SCOPE:
      break;
  }

  if (is_or_replace_) {
    modifiers.push_back("is_or_replace");
  }
  if (is_if_not_exists_) {
    modifiers.push_back("is_if_not_exists");
  }
  if (modifiers.empty()) {
    return ASTNode::SingleNodeDebugString();
  } else {
    return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                        absl::StrJoin(modifiers, ", "), ")");
  }
}

std::string ASTFunctionParameter::SingleNodeDebugString() const {
  std::vector<std::string> modifiers;
  if (is_not_aggregate()) {
    modifiers.push_back("is_not_aggregate=true");
  }
  if (procedure_parameter_mode() != ProcedureParameterMode::NOT_SET) {
    modifiers.push_back(absl::StrCat(
        "mode=", ProcedureParameterModeToString(procedure_parameter_mode())));
  }
  if (modifiers.empty()) {
    return ASTNode::SingleNodeDebugString();
  } else {
    return absl::StrCat(ASTNode::SingleNodeDebugString(), "(",
                        absl::StrJoin(modifiers, ", "), ")");
  }
}

// static
std::string ASTFunctionParameter::ProcedureParameterModeToString(
    ProcedureParameterMode mode) {
  switch (mode) {
    case ProcedureParameterMode::IN:
      return "IN";
    case ProcedureParameterMode::OUT:
      return "OUT";
    case ProcedureParameterMode::INOUT:
      return "INOUT";
    case ProcedureParameterMode::NOT_SET:
      return "";
  }
}

bool ASTFunctionParameter::IsTableParameter() const {
  return (tvf_schema_ != nullptr ||
          (templated_parameter_type_ != nullptr &&
           templated_parameter_type_->kind() ==
               ASTTemplatedParameterType::ANY_TABLE));
}

bool ASTFunctionDeclaration::IsTemplated() const {
  for (const ASTFunctionParameter* parameter :
           parameters()->parameter_entries()) {
    if (parameter->templated_parameter_type() != nullptr) {
      return true;
    }
  }
  return false;
}

std::string ASTCreateFunctionStmtBase::SingleNodeDebugString() const {
  return ASTCreateStatement::SingleNodeDebugString();
}

static std::string SqlForSqlSecurity(ASTCreateStatement::SqlSecurity sql_security) {
  switch (sql_security) {
    case ASTCreateStatement::SQL_SECURITY_INVOKER:
      return "SQL SECURITY INVOKER";
    case ASTCreateStatement::SQL_SECURITY_DEFINER:
      return "SQL SECURITY DEFINER";
    case ASTCreateStatement::SQL_SECURITY_UNSPECIFIED:
      return "";
  }
}

std::string ASTCreateFunctionStatement::SingleNodeDebugString() const {
  std::string aggregate = is_aggregate() ? "(is_aggregate=true)" : "";
  std::string security_str = sql_security() != SQL_SECURITY_UNSPECIFIED
                            ? absl::StrCat("(", GetSqlForSqlSecurity(), ")")
                            : "";
  return absl::StrCat(ASTCreateFunctionStmtBase::SingleNodeDebugString(),
                      aggregate, security_str);
}

std::string ASTCreateFunctionStmtBase::GetSqlForSqlSecurity() const {
  return SqlForSqlSecurity(sql_security());
}

std::string ASTCreateViewStatementBase::SingleNodeDebugString() const {
  std::string security_str = sql_security() != SQL_SECURITY_UNSPECIFIED
                            ? absl::StrCat("(", GetSqlForSqlSecurity(), ")")
                            : "";
  return absl::StrCat(ASTCreateStatement::SingleNodeDebugString(),
                      security_str);
}

std::string ASTCreateViewStatementBase::GetSqlForSqlSecurity() const {
  return SqlForSqlSecurity(sql_security());
}

std::string ASTCreateTableFunctionStatement::SingleNodeDebugString() const {
  return ASTCreateFunctionStmtBase::SingleNodeDebugString();
}

// Validates that 'target_path' is an ASTPathExpression (which is required for a
// non-nested DML statement). In that case, returns 'target_path' as an
// ASTPathExpression. Otherwise returns an error based on
// 'statement_type'.
static ::zetasql_base::StatusOr<const ASTPathExpression*>
GetTargetPathForNonNestedDMLStatement(
    absl::string_view statement_type,
    const ASTGeneralizedPathExpression* target_path) {
  ZETASQL_DCHECK_OK(ASTGeneralizedPathExpression::VerifyIsPureGeneralizedPathExpression(
      target_path));
  if (target_path->node_kind() == AST_PATH_EXPRESSION) {
    return target_path->GetAs<ASTPathExpression>();
  }

  // Find the parent node of the ASTPathExpression and use its right hand side
  // as the AST location for the error message.
  const ASTExpression* expr = target_path;
  const ASTNode* expr_rhs;
  while (true) {
    const ASTExpression* expr_lhs;

    switch (expr->node_kind()) {
      case AST_DOT_GENERALIZED_FIELD: {
        const auto* dot_generalized_field =
            expr->GetAs<ASTDotGeneralizedField>();
        expr_lhs = dot_generalized_field->expr();
        expr_rhs = dot_generalized_field->path();
        break;
      }
      case AST_DOT_IDENTIFIER: {
        const auto* dot_identifier = expr->GetAs<ASTDotIdentifier>();
        expr_lhs = dot_identifier->expr();
        expr_rhs = dot_identifier->name();
        break;
      }
      case AST_ARRAY_ELEMENT: {
        const auto* array_element = expr->GetAs<ASTArrayElement>();
        expr_lhs = array_element->array();
        expr_rhs = array_element->position();
        break;
      }
      default:
        ZETASQL_RET_CHECK_FAIL() << "Unexpected node kind in "
                         << "GetTargetPathForNonNestedDMLStatement(): "
                         << target_path->GetNodeKindString();
    }

    if (expr_lhs->node_kind() == AST_PATH_EXPRESSION) {
      break;
    }
    expr = expr_lhs;
  }

  return MakeSqlErrorAt(expr_rhs) << "Non-nested " << statement_type
                                  << " statement requires a table name";
}

zetasql_base::StatusOr<const ASTPathExpression*>
ASTDeleteStatement::GetTargetPathForNonNested() const {
  return GetTargetPathForNonNestedDMLStatement(/*statement_type=*/"DELETE",
                                               target_path_);
}

std::string ASTInsertStatement::SingleNodeDebugString() const {
  if (insert_mode_ == DEFAULT_MODE) {
    return ASTNode::SingleNodeDebugString();
  } else {
    return absl::StrCat(ASTNode::SingleNodeDebugString(),
                        "(insert_mode=", GetSQLForInsertMode(), ")");
  }
}

std::string ASTInsertStatement::GetSQLForInsertMode() const {
  switch (insert_mode_) {
    case DEFAULT_MODE:
      return "";
    case REPLACE:
      return "REPLACE";
    case UPDATE:
      return "UPDATE";
    case IGNORE:
      return "IGNORE";
  }
}

zetasql_base::StatusOr<const ASTPathExpression*>
ASTInsertStatement::GetTargetPathForNonNested() const {
  return GetTargetPathForNonNestedDMLStatement(/*statement_type=*/"INSERT",
                                               target_path_);
}

zetasql_base::StatusOr<const ASTPathExpression*>
ASTUpdateStatement::GetTargetPathForNonNested() const {
  return GetTargetPathForNonNestedDMLStatement(/*statement_type=*/"UPDATE",
                                               target_path_);
}

std::string ASTMergeAction::SingleNodeDebugString() const {
  const char* action_mode_string = nullptr;
  switch (action_type_) {
    case INSERT:
      action_mode_string = "INSERT";
      break;
    case UPDATE:
      action_mode_string = "UPDATE";
      break;
    case DELETE:
      action_mode_string = "DELETE";
      break;
    case NOT_SET:
      action_mode_string = "<INVALID ACTION MODE>";
      break;
  }
  return absl::StrCat(ASTNode::SingleNodeDebugString(), "(", action_mode_string,
                      ")");
}

std::string ASTMergeWhenClause::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(),
                      "(match_type=", GetSQLForMatchType(), ")");
}

std::string ASTMergeWhenClause::GetSQLForMatchType() const {
  switch (match_type_) {
    case MATCHED:
      return "MATCHED";
    case NOT_MATCHED_BY_SOURCE:
      return "NOT_MATCHED_BY_SOURCE";
    case NOT_MATCHED_BY_TARGET:
      return "NOT_MATCHED_BY_TARGET";
    case NOT_SET:
      LOG(DFATAL) << "Match type of merge match clause is not set.";
      return "";
  }
}

std::string ASTSampleSize::GetSQLForUnit() const {
  DCHECK_NE(unit_, NOT_SET);
  if (unit_ == NOT_SET) return "<UNKNOWN UNIT>";
  return unit_ == ROWS ? "ROWS" : "PERCENT";
}

std::string ASTGeneratedColumnInfo::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(),
                      is_stored_ ? "(is_stored=true)" : "");
}

std::string ASTNotNullColumnAttribute::SingleNodeSqlString() const {
  return "NOT NULL";
}

std::string ASTHiddenColumnAttribute::SingleNodeSqlString() const {
  return "HIDDEN";
}

std::string ASTPrimaryKeyColumnAttribute::SingleNodeSqlString() const {
  return "PRIMARY KEY";
}

std::string ASTForeignKeyColumnAttribute::SingleNodeSqlString() const {
  return "REFERENCES";
}

bool ASTColumnSchema::ContainsAttribute(ASTNodeKind node_kind) const {
  if (attributes() == nullptr) {
    return false;
  }
  for (const ASTColumnAttribute* attribute : attributes()->values()) {
    if (attribute->node_kind() == node_kind) {
      return true;
    }
  }
  return false;
}

std::string ASTCreateIndexStatement::SingleNodeDebugString() const {
  if (is_unique_) {
    return absl::StrCat(ASTNode::SingleNodeDebugString(), "(UNIQUE)");
  } else {
    return ASTNode::SingleNodeDebugString();
  }
}

std::string ASTForeignKeyReference::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(),
                      "(MATCH ", GetSQLForMatch(),
                      (enforced_ ? " " : " NOT "), "ENFORCED)");
}

std::string ASTForeignKeyReference::GetSQLForMatch() const {
  switch (match_) {
    case SIMPLE:
      return "SIMPLE";
    case FULL:
      return "FULL";
    case NOT_DISTINCT:
      return "NOT DISTINCT";
  }
}

std::string ASTForeignKeyActions::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(),
                      "(ON UPDATE ", GetSQLForAction(update_action_),
                      " ON DELETE ", GetSQLForAction(delete_action_),
                      ")");
}

std::string ASTForeignKeyActions::GetSQLForAction(Action action) {
  switch (action) {
    case NO_ACTION:
      return "NO ACTION";
    case RESTRICT:
      return "RESTRICT";
    case CASCADE:
      return "CASCADE";
    case SET_NULL:
      return "SET NULL";
  }
}

std::string ASTCheckConstraint::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(),
                      is_enforced_ ? "(ENFORCED)" : "(NOT ENFORCED)");
}

std::string ASTSetOptionsAction::GetSQLForAlterAction() const {
  return "SET OPTIONS";
}

std::string ASTAddConstraintAction::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(),
                      is_if_not_exists() ? "(is_if_not_exists)" : "");
}

std::string ASTAddConstraintAction::GetSQLForAlterAction() const {
  return "ADD CONSTRAINT";
}

std::string ASTDropConstraintAction::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(),
                      is_if_exists() ? "(is_if_exists)" : "");
}

std::string ASTDropConstraintAction::GetSQLForAlterAction() const {
  return "DROP CONSTRAINT";
}

std::string ASTAlterConstraintSetOptionsAction::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(),
                      is_if_exists() ? "(is_if_exists)" : "");
}

std::string ASTAlterConstraintSetOptionsAction::GetSQLForAlterAction() const {
  return "ALTER CONSTRAINT SET OPTIONS";
}

std::string ASTAlterConstraintEnforcementAction::SingleNodeDebugString() const {
  return absl::StrCat(ASTNode::SingleNodeDebugString(),
                      is_if_exists() ? "(is_if_exists)" : "");
}

std::string ASTAlterConstraintEnforcementAction::GetSQLForAlterAction() const {
  return "ALTER CONSTRAINT [NOT] ENFORCED";
}

std::string ASTAlterStatementBase::SingleNodeDebugString() const {
  const std::string& node_name = ASTNode::SingleNodeDebugString();
  if (!is_if_exists()) {
    return node_name;
  }
  return absl::StrCat(node_name, "(is_if_exists)");
}

std::string SchemaObjectKindToName(SchemaObjectKind schema_object_kind) {
  switch (schema_object_kind) {
    case(SchemaObjectKind::kAggregateFunction):
      return "AGGREGATE FUNCTION";
    case(SchemaObjectKind::kConstant):
      return "CONSTANT";
    case(SchemaObjectKind::kDatabase):
      return "DATABASE";
    case(SchemaObjectKind::kExternalTable):
      return "EXTERNAL TABLE";
    case(SchemaObjectKind::kFunction):
      return "FUNCTION";
    case(SchemaObjectKind::kIndex):
      return "INDEX";
    case(SchemaObjectKind::kMaterializedView):
      return "MATERIALIZED VIEW";
    case(SchemaObjectKind::kModel):
      return "MODEL";
    case(SchemaObjectKind::kProcedure):
      return "PROCEDURE";
    case(SchemaObjectKind::kTable):
      return "TABLE";
    case(SchemaObjectKind::kTableFunction):
      return "TABLE FUNCTION";
    case(SchemaObjectKind::kView):
      return "VIEW";
    default:
      return "<INVALID SCHEMA OBJECT KIND>";
  }
}

}  // namespace zetasql
