/* SPDX-License-Identifier: MPL-2.0
 * Copyright (C) 2026 Charles Vestal */
/* Ctl settings: one flash object (OBJ_PROJ). Settings of another format, or none, give the defaults.
 * Part of the unity build, after app.h. */
project_t proj;

/* the knob banks: A filter cutoff, resonance, modulation, volume; B attack, release, decay, vibrato;
 * C and D the general-purpose controllers 16-23 */
static const uint8_t CC_DEFAULT[NBANKS][NKNOBS] = {{74, 71, 1, 7}, {73, 72, 75, 76}, {16, 17, 18, 19}, {20, 21, 22, 23}};
/* the sequencer: General MIDI drums (kick, snare, closed hat, clap) */
static const uint8_t TRK_DEFAULT[NTRACKS][4] = {{16, 4, 0, 36}, {16, 2, 4, 38}, {16, 8, 0, 42}, {12, 3, 2, 39}};
/* channel names: first 5 alphanumeric characters of the original chan_names[], special chars stripped */
// should be strictly 5 characters each + 1 byte for color index (0-6).
// the trailing spaces will be striped when displayed
static const char CHAN_NAME_DEFAULT[16][6] = {
    {'L', 'O', 'G', 'I', 'C', 0},  /* LOGIC: K_BLUE   (0) */
    {'T', 'R', 'A', 'C', 'K', 3},  /* TRACK: K_RED    (3) */
    {'S', 'E', 'N', 'D', ' ', 1},  /* SEND : K_TEAL   (1) */
    {'S', 'M', 'A', 'R', 'T', 4},  /* SMART: K_ORANGE (4) */
    {'E', 'Q', ' ', '1', ' ', 2},  /* EQ 1 : K_GREEN  (2) */
    {'E', 'Q', ' ', '2', ' ', 5},  /* EQ 2 : K_GREY   (5) */
    {'E', 'Q', ' ', '3', ' ', 6},  /* EQ 3 : K_BLACK  (6) */
    {'C', 'M', 'P', ' ', '1', 0},  /* CMP 1: K_BLUE   (0) */
    {'C', 'M', 'P', ' ', '2', 3},  /* CMP 2: K_RED    (3) */
    {'C', 'M', 'P', ' ', '3', 1},  /* CMP 3: K_TEAL   (1) */
    {'A', 'M', 'P', ' ', ' ', 4},  /* AMP  : K_ORANGE (4) */
    {'G', 'L', 'O', 'W', ' ', 2},  /* GLOW : K_GREEN  (2) */
    {'G', 'A', 'I', 'N', ' ', 5},  /* GAIN : K_GREY   (5) */
    {'E', 'X', 'T', 'R', 'A', 6},  /* EXTRA: K_BLACK  (6) */
    {'V', 'I', 'D', 'E', 'O', 0},  /* VIDEO: K_BLUE   (0) */
    {'R', 'E', 'C', ' ', ' ', 3}   /* REC  : K_RED    (3) */
};

/* hits spread as evenly as possible over steps (the Euclidean rhythm), turned right by rot */
uint16_t euclid(int steps, int hits, int rot)
{
    uint16_t p = 0;
    int i;
    if (steps < 1 || steps > 16)
        return 0;
    if (hits > steps)
        hits = steps;
    for (i = 0; i < steps; i++)
        if ((i * hits) % steps < hits)
            p |= (uint16_t)(1u << ((i + rot) % steps));
    return p;
}

void project_defaults(void)
{
    int b, k;
    memset(&proj, 0, sizeof proj);
    proj.magic = PROJ_MAGIC;
    proj.format = PROJ_FORMAT;
    proj.vel = 100;
    for (b = 0; b < NBANKS; b++)
        for (k = 0; k < NKNOBS; k++) {
            proj.cc[b][k] = CC_DEFAULT[b][k];
            proj.val[b][k] = CC_DEFAULT[b][k] == 7 ? 100 : CC_DEFAULT[b][k] == 1 ? 0 : 64;
        }
    proj.scale = 0;                              /* chromatic: the keys as a keyboard (root C for a scale) */
    // set default knob bank to C (logic mode)
    proj.bank = 2;
    proj.tempo = 120;
    proj.clock = CLK_EXT;
    proj.arp_rate = RATE_16;
    proj.arp_oct = 1;
    proj.seq_ch = 9;                             /* channel 10 */
    proj.glow = 1;
    proj.touch = 1;                              /* velocity from the keys */
    proj.at_mode = AT_POLY;                      /* and aftertouch, per note */
    for (k = 0; k < NTRACKS; k++) {
        track_t *t = &proj.trk[k];
        t->steps = TRK_DEFAULT[k][0];
        t->hits = TRK_DEFAULT[k][1];
        t->rot = TRK_DEFAULT[k][2];
        t->note = TRK_DEFAULT[k][3];
        t->pat = euclid(t->steps, t->hits, t->rot);
    }
    for (k = 0; k < 16; k++)
        memcpy(proj.chan_name[k], CHAN_NAME_DEFAULT[k], 6);
}

static int project_valid(void)
{
    int b, k;
    if (proj.magic != PROJ_MAGIC || proj.format != PROJ_FORMAT)
        return 0;
    if (proj.chan > 15u || proj.vel < 1u || proj.vel > 127u || proj.octave < -3 || proj.octave > 3 ||
        proj.program > 127u || proj.bank >= NBANKS || proj.root > 11u || proj.scale >= NSCALES ||
        proj.human > 40u || proj.tempo < 40u || proj.tempo > 240u || proj.clock >= NCLK || proj.arp_on > 1u ||
        proj.arp_mode >= NARP || proj.arp_rate >= NRATE || proj.arp_oct < 1u || proj.arp_oct > 3u ||
        proj.arp_latch > 1u || proj.seq_ch > 15u || proj.master_cc > 119u || proj.glow > 2u ||
        proj.sound >= NSOUND || proj.in_mute > 1u || proj.touch > 1u || proj.at_mode >= NAT ||
        proj.vel_curve < -5 || proj.vel_curve > 5 || proj.at_sens < -5 || proj.at_sens > 5)
        return 0;
    for (b = 0; b < NBANKS; b++)
        for (k = 0; k < NKNOBS; k++)
            if (proj.cc[b][k] > 119u || proj.val[b][k] > 127u)
                return 0;
    for (k = 0; k < NTRACKS; k++) {
        const track_t *t = &proj.trk[k];
        if (t->steps < 1u || t->steps > 16u || t->hits > t->steps || t->rot >= t->steps || t->note > 127u ||
            t->mute > 1u)
            return 0;
    }
    for (k = 0; k < 16; k++) {
        int j;
        if ((uint8_t)proj.chan_name[k][5] > 6u)
            return 0;
        for (j = 0; j < 5; j++) {
            char c = proj.chan_name[k][j];
            if (!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' '))
                return 0;
        }
    }
    return 1;
}

int project_load(void)
{
    int n;
    memset(&proj, 0, sizeof proj);
    n = plat_store_load(OBJ_PROJ, &proj, sizeof proj);
    if (n >= 8 && project_valid()) {
        if (proj.sound == SOUND_KEYS) {          /* an older project's "Keys": on, MIDI in not played */
            proj.sound = SOUND_IN;
            proj.in_mute = 1;
        }
        return 0;
    }
    project_defaults();
    return -1;
}

int project_save(void) { return plat_store_save(OBJ_PROJ, &proj, sizeof proj); }
