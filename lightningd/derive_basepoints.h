#ifndef LIGHTNING_LIGHTNINGD_DERIVE_BASEPOINTS_H
#define LIGHTNING_LIGHTNINGD_DERIVE_BASEPOINTS_H
#include "config.h"
#include <bitcoin/privkey.h>
#include <bitcoin/pubkey.h>

struct sha256;

struct basepoints {
	struct pubkey revocation;
	struct pubkey payment;
	struct pubkey delayed_payment;
};

struct secrets {
	struct privkey funding_privkey;
	struct privkey revocation_basepoint_secret;
	struct privkey payment_basepoint_secret;
	struct privkey delayed_payment_basepoint_secret;
};

bool derive_basepoints(const struct privkey *seed,
		       struct pubkey *funding_pubkey,
		       struct basepoints *basepoints,
		       struct secrets *secrets,
		       struct sha256 *shaseed,
		       struct pubkey *per_commit_point,
		       u64 per_commit_index);

/* Give up secret for index-1, and generate per-commitment point for N+1. */
bool next_per_commit_point(const struct sha256 *shaseed,
			   struct sha256 *old_commit_secret,
			   struct pubkey *per_commit_point,
			   u64 per_commit_index);

/* BOLT #3:
 *
 * the first secret used MUST be index 281474976710655, and then the index
 * decremented.
 */
static inline u64 shachain_index(u64 per_commit_index)
{
	assert(per_commit_index < (1ULL << 48));
	return 281474976710655ULL - per_commit_index;
}
#endif /* LIGHTNING_LIGHTNINGD_DERIVE_BASEPOINTS_H */
