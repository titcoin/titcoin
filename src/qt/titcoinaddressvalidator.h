// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef TITCOIN_QT_TITCOINADDRESSVALIDATOR_H
#define TITCOIN_QT_TITCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class TitcoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit TitcoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Titcoin address widget validator, checks for a valid titcoin address.
 */
class TitcoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit TitcoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // TITCOIN_QT_TITCOINADDRESSVALIDATOR_H
