// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Copyright (c) 2014-2020 The Titcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
//#include <inttypes.h>
//#include <util.h>

unsigned int GetNextWorkRequiredV1(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacingV1*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

unsigned int GetNextWorkRequiredV2(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    // Transferred from https://github.com/titcoin/titcoin-original/blob/master/src/main.cpp#L1177
    // Modified Digishield. Digishield retargets every block, reducing the apparent difference between the actual
    // block interval and target block interval by a factor of 8. It allows a 33% change in difficulty.

    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    if (pindexLast->nHeight == 0) // Genesis block
        return nProofOfWorkLimit;

    // Titcoin: Digishield implementation means difficulty changes every block

    // Go back by one block
    const CBlockIndex* pindexFirst = pindexLast->pprev;
    assert(pindexFirst);

    return CalculateNextWorkRequiredV2(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequiredV2(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting) // regtest
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    //LogPrintf("  nActualTimespan = %" PRId64 " before bounds\n", nActualTimespan);

    nActualTimespan = params.nPowTargetSpacing + (nActualTimespan - params.nPowTargetSpacing) / 8;

    int64_t lowerLim = params.nPowTargetSpacing - params.nPowTargetSpacing / 4;  // 45
    int64_t higherLim = params.nPowTargetSpacing + params.nPowTargetSpacing / 2; // 90

    if (nActualTimespan < lowerLim) // can only happen with a negative time difference, since 60 + (0 - 60)/8 = 52.5 > 45
	    nActualTimespan = lowerLim;
    else if (nActualTimespan > higherLim) // there hasn't been a block for 5 minutes
	    nActualTimespan = higherLim;

    // Retarget: Scale up or down the difficulty every block proprotionally to the target delta with a smoothing factor of 1/8.
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetSpacing;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    /// debug print
    //LogPrintf("GetNextWorkRequiredV2 RETARGET\n");
    //LogPrintf("nTargetTimespan = %" PRId64 "   nActualTimespan (adjusted) = %" PRId64 "\n", params.nPowTargetSpacing, nActualTimespan);
    //LogPrintf("Before: %08x  %s\n", pindexLast->nBits, ArithToUint256(arith_uint256().SetCompact(pindexLast->nBits)).ToString());
    //LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), ArithToUint256(bnNew).ToString());
    return bnNew.GetCompact();
}


unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    return ((pindexLast->nTime < params.DifficultyForkTime) ? GetNextWorkRequiredV1 : GetNextWorkRequiredV2) (pindexLast, pblock, params);
}


bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    // Disable check for the genesis block because the Titcoin genesis block does not meet the minimum diff
    if (hash != params.hashGenesisBlock && UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
