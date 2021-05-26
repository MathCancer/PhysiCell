/*
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2011-2020 Institut Curie, 26 rue d'Ulm, Paris, France       #
# All rights reserved.                                                      #
#                                                                           #
# Redistribution and use in source and binary forms, with or without        #
# modification, are permitted provided that the following conditions are    #
# met:                                                                      #
#                                                                           #
# 1. Redistributions of source code must retain the above copyright notice, #
# this list of conditions and the following disclaimer.                     #
#                                                                           #
# 2. Redistributions in binary form must reproduce the above copyright      #
# notice, this list of conditions and the following disclaimer in the       #
# documentation and/or other materials provided with the distribution.      #
#                                                                           #
# 3. Neither the name of the copyright holder nor the names of its          #
# contributors may be used to endorse or promote products derived from this #
# software without specific prior written permission.                       #
#                                                                           #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           #
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER #
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    #
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      #
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              #
#                                                                           #
#############################################################################

   Module:
     BooleanNetwork.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     October 2014
*/

#include "BooleanNetwork.h"
#include "BooleanGrammar.h"
#include "RunConfig.h"
#include <iostream>

static const std::string LOGICAL_EXPR_MSG = "MaBoSS: logical expression generation: ";

bool dont_shrink_logical_expressions;
static bool rewrite_xor = true;

void Network::generateLogicalExpressions(std::ostream& os) const
{
  std::vector<Node*>::const_iterator begin = nodes.begin();
  std::vector<Node*>::const_iterator end = nodes.end();

  for (unsigned int nn = 0; begin != end; ++nn) {
    Node* node = *begin;
    LogicalExprGenContext genctx(this, node, os);
    node->generateLogicalExpression(genctx);
    os << '\n';
    ++begin;
  }
}

Expression* Expression::cloneAndShrinkRecursive(Expression* expr)
{
  Expression* shrinked_expr = expr;
  for (;;) {
    bool shrinked = false;
    Expression* shrinked_expr_new = shrinked_expr->cloneAndShrink(shrinked);
    delete shrinked_expr;
    shrinked_expr = shrinked_expr_new;
    if (!shrinked) {
      return shrinked_expr;
    }
  }
  return NULL;
}

Expression* Node::rewriteLogicalExpression(Expression* ref_rateUpExpr, Expression* ref_rateDownExpr) const
{
  return
    new OrLogicalExpression
    (
     new AndLogicalExpression
     (
      new NotLogicalExpression(new NodeExpression(const_cast<Node*>(this))),
      ref_rateUpExpr
      ),
     new AndLogicalExpression
     (
      new NodeExpression(const_cast<Node*>(this)),
      new NotLogicalExpression(ref_rateDownExpr)
      )
     );
}

void Node::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  Expression* rewrited_expr = NULL;
  if (NULL != rateUpExpr && NULL != rateDownExpr) {
    rewrited_expr = rewriteLogicalExpression(rateUpExpr->clone(), rateDownExpr->clone());
  } else if (NULL != rateUpExpr) {
    if (NULL == logicalInputExpr) {
      rewrited_expr = rewriteLogicalExpression(rateUpExpr->clone(), new ConstantExpression(0.0));
    } else {
      rewrited_expr = rewriteLogicalExpression(rateUpExpr->clone(), new NotLogicalExpression(const_cast<Expression*>(logicalInputExpr)));
    }
  } else if (NULL != rateDownExpr) {
    if (NULL == logicalInputExpr) {
      rewrited_expr = rewriteLogicalExpression(new ConstantExpression(1.0), rateDownExpr->clone());
    } else {
      rewrited_expr = rewriteLogicalExpression(logicalInputExpr->clone(), rateDownExpr->clone());
    }
  } else if (NULL != logicalInputExpr) {
    rewrited_expr = logicalInputExpr->clone();
  } else {
    rewrited_expr = new ConstantExpression(0.);
  }

  Expression* shrinked_expr = Expression::cloneAndShrinkRecursive(rewrited_expr);
  
  std::ostream& os = genctx.getOStream();
  os << label << " : ";

  shrinked_expr->generateLogicalExpression(genctx);
  delete shrinked_expr;
}

bool Expression::evalIfConstant(double& value) const
{
  if (dont_shrink_logical_expressions) {
    return false;
  }

  if (isConstantExpression()) {
    NetworkState network_state;
    value = eval(NULL, network_state);
    return true;
  }
  return false;
}

bool Expression::evalIfConstant(bool& value) const
{
  double d_value;
  if (evalIfConstant(d_value)) {
    value = (bool)d_value;
    return true;
  }
  return false;
}

Expression* CondExpression::cloneAndShrink(bool& shrinked) const
{
  shrinked = true;
 
  bool cond_value;
  if (cond_expr->evalIfConstant(cond_value)) {
    return cond_value ? true_expr->clone() : false_expr->clone();
  }

  return
    new OrLogicalExpression
    (
     new AndLogicalExpression
     (
      cond_expr->clone(),
      true_expr->clone()
      ),
     new AndLogicalExpression
     (
      new NotLogicalExpression(cond_expr->clone()),
      false_expr->clone()
      )
     );
}

void CondExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  throw BNException(LOGICAL_EXPR_MSG + "CondExpression INTERNAL ERROR");
}

void ConstantExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  std::ostream& os = genctx.getOStream();
  os << (value != 0 ? 1 : 0);
}

void SymbolExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  std::ostream& os = genctx.getOStream();
  double value = symbol_table->getSymbolValue(symbol);
  os << (value != 0 ? 1 : 0);
}

void ParenthesisExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  LogicalExprGenContext::LevelManager levelManager(genctx);
  unsigned int level = levelManager.getLevel();

  std::ostream& os = genctx.getOStream();
  if (level && !expr->generationWillAddParenthesis()) {
    os << '(';
  }
  expr->generateLogicalExpression(genctx);
  if (level && !expr->generationWillAddParenthesis()) {
    os << ')';
  }
}

void NotLogicalExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  LogicalExprGenContext::LevelManager levelManager(genctx);

  std::ostream& os = genctx.getOStream();
  bool b_expr;
  if (expr->evalIfConstant(b_expr)) {
    os << b_expr;
  } else {
    if (!dont_shrink_logical_expressions) {
      const NotLogicalExpression* not_expr = expr->asNotLogicalExpression();

      if (NULL != not_expr) {
	not_expr->expr->generateLogicalExpression(genctx);
	return;
      }
    }
    os << LOGICAL_NOT_SYMBOL;
    expr->generateLogicalExpression(genctx);
  }
}

Expression* NotLogicalExpression::cloneAndShrink(bool& shrinked) const
{
  bool b_expr;
  if (expr->evalIfConstant(b_expr)) {
    shrinked = true;
    return new ConstantExpression(b_expr);
  }
  return new NotLogicalExpression(expr->cloneAndShrink(shrinked));
}

void OrLogicalExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  LogicalExprGenContext::LevelManager levelManager(genctx);
  unsigned int level = levelManager.getLevel();

  std::ostream& os = genctx.getOStream();
  bool b_left, b_right;

  if (left->evalIfConstant(b_left)) {
    if (b_left) {
      os << '1';
    } else {
      right->generateLogicalExpression(genctx);
    }
  } else if (right->evalIfConstant(b_right)) {
    if (b_right) {
      os << '1';
    } else {
      left->generateLogicalExpression(genctx);
    }
  } else {
    if (level) {
      os << '(';
    }
    left->generateLogicalExpression(genctx);
    os << LOGICAL_OR_SYMBOL;
    right->generateLogicalExpression(genctx);
    if (level) {
      os << ')';
    }
  }
}

Expression* OrLogicalExpression::cloneAndShrink(bool& shrinked) const
{
  bool b_left;
  if (left->evalIfConstant(b_left)) {
    shrinked = true;
    if (b_left) {
      return new ConstantExpression(1);
    }
    return right->cloneAndShrink(shrinked);
  }
  bool b_right;
  if (right->evalIfConstant(b_right)) {
    shrinked = true;
    if (b_right) {
      return new ConstantExpression(1);
    }
    return left->cloneAndShrink(shrinked);
  }
  return new OrLogicalExpression(left->cloneAndShrink(shrinked), right->cloneAndShrink(shrinked));
}

void AndLogicalExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  LogicalExprGenContext::LevelManager levelManager(genctx);
  unsigned int level = levelManager.getLevel();

  std::ostream& os = genctx.getOStream();
  bool b_left, b_right;

  if (left->evalIfConstant(b_left)) {
    if (!b_left) {
      os << '0';
    } else {
      right->generateLogicalExpression(genctx);
    }
  } else if (right->evalIfConstant(b_right)) {
    if (!b_right) {
      os << '0';
    } else {
      left->generateLogicalExpression(genctx);
    }
  } else {
    if (level) {
      os << '(';
    }
    left->generateLogicalExpression(genctx);
    os << LOGICAL_AND_SYMBOL;
    right->generateLogicalExpression(genctx);
    if (level) {
      os << ')';
    }
  }
}

Expression* AndLogicalExpression::cloneAndShrink(bool& shrinked) const
{
  bool b_left;
  if (left->evalIfConstant(b_left)) {
    shrinked = true;
    if (!b_left) {
      return new ConstantExpression(0);
    }
    return right->cloneAndShrink(shrinked);
  }
  bool b_right;
  if (right->evalIfConstant(b_right)) {
    shrinked = true;
    if (!b_right) {
      return new ConstantExpression(0);
    }
    return left->cloneAndShrink(shrinked);
  }
  return new AndLogicalExpression(left->cloneAndShrink(shrinked), right->cloneAndShrink(shrinked));
}

Expression* XorLogicalExpression::cloneAndShrink(bool& shrinked) const
{
  if (rewrite_xor) {
    shrinked = true;
    return
      new AndLogicalExpression
      (
       new OrLogicalExpression
       (
	left->clone(),
	right->clone()
	),
       new NotLogicalExpression
       (
	new AndLogicalExpression
	(
	left->clone(),
	right->clone()
	 )
	)
       );
  }

  bool b_left;
  if (left->evalIfConstant(b_left)) {
    shrinked = true;
    if (b_left) {
      return new NotLogicalExpression(right->cloneAndShrink(shrinked));
    }
    return right->cloneAndShrink(shrinked);
  }
  bool b_right;
  if (right->evalIfConstant(b_right)) {
    shrinked = true;
    if (b_right) {
      return new NotLogicalExpression(left->cloneAndShrink(shrinked));
    }
    return left->cloneAndShrink(shrinked);
  }
  return new XorLogicalExpression(left->cloneAndShrink(shrinked), right->cloneAndShrink(shrinked));
}

void XorLogicalExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  std::ostream& os = genctx.getOStream();
  if (rewrite_xor) {
    throw BNException(LOGICAL_EXPR_MSG + "XorLogicalExpression INTERNAL ERROR");
  }
  left->generateLogicalExpression(genctx);
  os << LOGICAL_XOR_SYMBOL;
  right->generateLogicalExpression(genctx);
}

void NodeExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  std::ostream& os = genctx.getOStream();
  os << node->getLabel();
}

void AliasExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  const Expression* alias_expr = getAliasExpression(genctx.getNode());
  if (NULL == alias_expr) {
    throw BNException("invalid use of alias attribute @" + identifier + " in node " + genctx.getNode()->getLabel());
  }

  Expression* shrinked_expr = cloneAndShrinkRecursive(alias_expr->clone());
  shrinked_expr->generateLogicalExpression(genctx);
  delete shrinked_expr;
}

// NOT IMPLEMENTED generateLogicalExpression methods
void MulExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  throw BNException(LOGICAL_EXPR_MSG + "MulExpression is not yet implemented");
}

void AddExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  throw BNException(LOGICAL_EXPR_MSG + "AddExpression is not yet implemented");
}

void SubExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  throw BNException(LOGICAL_EXPR_MSG + "SubExpression is not yet implemented");
}

void DivExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  throw BNException(LOGICAL_EXPR_MSG + "DivExpression is not yet implemented");
}

void EqualExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  throw BNException(LOGICAL_EXPR_MSG + "EqualExpression is not yet implemented");
}

void NotEqualExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  throw BNException(LOGICAL_EXPR_MSG + "NotEqualExpression is not yet implemented");
}

void LetterExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  throw BNException(LOGICAL_EXPR_MSG + "LetterExpression is not yet implemented");
}

void LetterOrEqualExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  throw BNException(LOGICAL_EXPR_MSG + "LetterOrEqualExpression is not yet implemented");
}

void GreaterExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  throw BNException(LOGICAL_EXPR_MSG + "GreaterExpression is not yet implemented");
}

void GreaterOrEqualExpression::generateLogicalExpression(LogicalExprGenContext& genctx) const
{
  throw BNException(LOGICAL_EXPR_MSG + "GreaterOrEqualExpression is not yet implemented");
}
