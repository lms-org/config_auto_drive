#ifndef FISHER_BREAKS_H
#define FISHER_BREAKS_H

#include <iostream>
#include <assert.h>
#include <vector>
#include <algorithm>

typedef std::size_t                  SizeT;
typedef SizeT                        CountType;
typedef std::pair<double, CountType> ValueCountPair;
typedef std::vector<double>          LimitsContainer;
typedef std::vector<ValueCountPair>  ValueCountPairContainer;


/*
 * source code for Fisher breaks algorithm:
 * http://wiki.objectvision.nl/index.php/CalcNaturalBreaksCode
 *
 */

class FisherBreaks  {
public:
    static SizeT GetTotalCount(const  ValueCountPairContainer& vcpc);
    static void GetCountsDirect(ValueCountPairContainer& vcpc, const double* values, SizeT size);
    static void MergeToLeft(ValueCountPairContainer& vcpcLeft, const ValueCountPairContainer& vcpcRight, ValueCountPairContainer& vcpcDummy);
    static void GetValueCountPairs(ValueCountPairContainer& vcpc, const double* values, SizeT n);
    static void ClassifyJenksFisherFromValueCountPairs(LimitsContainer& breaksArray, SizeT k, const ValueCountPairContainer& vcpc);

private:

};

#endif // FISHER_BREAKS_H
