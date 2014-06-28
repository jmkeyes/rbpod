/* track.c */

#include "rbpod.h"

ID sMEDIA_TYPE_AUDIO_VIDEO;
ID sMEDIA_TYPE_AUDIO;
ID sMEDIA_TYPE_VIDEO;
ID sMEDIA_TYPE_PODCAST;
ID sMEDIA_TYPE_VIDEO_PODCAST;
ID sMEDIA_TYPE_AUDIO_BOOK;
ID sMEDIA_TYPE_MUSIC_VIDEO;
ID sMEDIA_TYPE_TV_SHOW_EXCL;
ID sMEDIA_TYPE_TV_SHOW;

typedef enum {
    MEDIA_TYPE_AUDIO_VIDEO   = 0x00000000,
    MEDIA_TYPE_AUDIO         = 0x00000001,
    MEDIA_TYPE_VIDEO         = 0x00000002,
    MEDIA_TYPE_PODCAST       = 0x00000004,
    MEDIA_TYPE_VIDEO_PODCAST = 0x00000006,
    MEDIA_TYPE_AUDIO_BOOK    = 0x00000008,
    MEDIA_TYPE_MUSIC_VIDEO   = 0x00000020,
    MEDIA_TYPE_TV_SHOW_EXCL  = 0x00000040,
    MEDIA_TYPE_TV_SHOW       = 0x00000060,
} media_type;

DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, title);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, album);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, artist);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, genre);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, filetype);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, comment);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, category);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, composer);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, grouping);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, description);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, podcasturl);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, podcastrss);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, subtitle);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, tvshow);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, tvepisode);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, tvnetwork);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, albumartist);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, keywords);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, sort_artist);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, sort_title);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, sort_album);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, sort_albumartist);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, sort_composer);
DEF_ATTR_ACCESSOR(Itdb_Track, track, STRING, sort_tvshow);

DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, id);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, size);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, tracklen);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, cd_nr);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, cds);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, track_nr);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, tracks);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, bitrate);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, samplerate);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, year);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, volume);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, soundcheck);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, rating);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, playcount);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, recent_playcount);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, BPM);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, starttime);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, stoptime);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, skipcount);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, recent_skipcount);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, pregap);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, samplecount);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, postgap);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, season_nr);
DEF_ATTR_ACCESSOR(Itdb_Track, track, FIXNUM, episode_nr);

DEF_ATTR_ACCESSOR(Itdb_Track, track, BOOLEAN, transferred);
DEF_ATTR_ACCESSOR(Itdb_Track, track, BOOLEAN, compilation);
DEF_ATTR_ACCESSOR(Itdb_Track, track, BOOLEAN, visible);
DEF_ATTR_ACCESSOR(Itdb_Track, track, BOOLEAN, explicit_flag);
DEF_ATTR_ACCESSOR(Itdb_Track, track, BOOLEAN, skip_when_shuffling);
DEF_ATTR_ACCESSOR(Itdb_Track, track, BOOLEAN, remember_playback_position);
DEF_ATTR_ACCESSOR(Itdb_Track, track, BOOLEAN, lyrics_flag);
DEF_ATTR_ACCESSOR(Itdb_Track, track, BOOLEAN, movie_flag);
DEF_ATTR_ACCESSOR(Itdb_Track, track, BOOLEAN, gapless_track_flag);
DEF_ATTR_ACCESSOR(Itdb_Track, track, BOOLEAN, gapless_album_flag);

DEF_ATTR_ACCESSOR(Itdb_Track, track, DATETIME, time_added);
DEF_ATTR_ACCESSOR(Itdb_Track, track, DATETIME, time_modified);
DEF_ATTR_ACCESSOR(Itdb_Track, track, DATETIME, time_played);
DEF_ATTR_ACCESSOR(Itdb_Track, track, DATETIME, bookmark_time);
DEF_ATTR_ACCESSOR(Itdb_Track, track, DATETIME, time_released);
DEF_ATTR_ACCESSOR(Itdb_Track, track, DATETIME, last_skipped);

static VALUE rbpod_track_ipod_path_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    gchar *buffer = NULL;

    if (track->ipod_path == NULL)
        return Qnil;

    buffer = g_strdup(track->ipod_path);

    itdb_filename_ipod2fs(buffer);

    return rb_str_new2(buffer);
}

static VALUE rbpod_track_ipod_path_set(VALUE self, VALUE value)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    gchar *new_path = StringValueCStr(value);
    gchar *buffer = NULL;

    if (track->ipod_path != NULL)
        g_free(track->ipod_path);

    buffer = g_strdup(new_path);

    itdb_filename_fs2ipod(buffer);

    track->ipod_path = buffer;

    return Qnil;
}

static VALUE rbpod_track_mediatype_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);

    switch(track->mediatype) {
        case MEDIA_TYPE_AUDIO_VIDEO:
            return rb_const_get(cRbPodTrack, sMEDIA_TYPE_AUDIO_VIDEO);
        case MEDIA_TYPE_AUDIO:
            return rb_const_get(cRbPodTrack, sMEDIA_TYPE_AUDIO);
        case MEDIA_TYPE_VIDEO:
            return rb_const_get(cRbPodTrack, sMEDIA_TYPE_VIDEO);
        case MEDIA_TYPE_PODCAST:
            return rb_const_get(cRbPodTrack, sMEDIA_TYPE_PODCAST);
        case MEDIA_TYPE_VIDEO_PODCAST:
            return rb_const_get(cRbPodTrack, sMEDIA_TYPE_VIDEO_PODCAST);
        case MEDIA_TYPE_AUDIO_BOOK:
            return rb_const_get(cRbPodTrack, sMEDIA_TYPE_AUDIO_BOOK);
        case MEDIA_TYPE_MUSIC_VIDEO:
            return rb_const_get(cRbPodTrack, sMEDIA_TYPE_MUSIC_VIDEO);
        case MEDIA_TYPE_TV_SHOW_EXCL:
            return rb_const_get(cRbPodTrack, sMEDIA_TYPE_TV_SHOW_EXCL);
        case MEDIA_TYPE_TV_SHOW:
            return rb_const_get(cRbPodTrack, sMEDIA_TYPE_TV_SHOW);
    }

    return Qnil;
}

static VALUE rbpod_track_mediatype_set(VALUE self, VALUE value)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    track->mediatype = NUM2INT(value);
    return Qnil;
}

static VALUE rbpod_track_checked_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return (track->checked == 0x0) ? Qtrue : Qfalse;
}

static VALUE rbpod_track_checked_set(VALUE self, VALUE value)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    track->checked = RTEST(value) ? 0x0 : 0x1;
    return Qnil;
}

static VALUE rbpod_track_has_artwork_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return (track->has_artwork == 0x01) ? Qtrue : Qfalse;
}

static VALUE rbpod_track_has_artwork_set(VALUE self, VALUE value)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    track->has_artwork = RTEST(value) ? 0x1 : 0x2;
    return Qnil;
}

static VALUE rbpod_track_mark_unplayed_get(VALUE self)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    return (track->mark_unplayed == 0x2) ? Qtrue : Qfalse;
}

static VALUE rbpod_track_mark_unplayed_set(VALUE self, VALUE value)
{
    Itdb_Track *track = TYPED_DATA_PTR(self, Itdb_Track);
    track->mark_unplayed = RTEST(value) ? 0x2 : 0x1;
    return Qnil;
}

/*
 * call-seq:
 *     initialize() -> RbPod::Track
 *
 * Creates a detached track. (Not managed by a database.)
 */
static VALUE rbpod_track_initialize(VALUE self)
{
    return self;
}

static void rbpod_track_deallocate(void *handle)
{
    Itdb_Track *track = (Itdb_Track *) handle;

    /* Don't free tracks that are assigned to a playlist/database. */
    if (track->itdb == NULL) {
        itdb_track_free((Itdb_Track *) handle);
    }

    return;
}

static VALUE rbpod_track_allocate(VALUE self)
{
    Itdb_Track *track = itdb_track_new();
    return Data_Wrap_Struct(cRbPodTrack, NULL, rbpod_track_deallocate, (void *) track);
}

void Init_rbpod_track(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    cRbPodTrack = rb_define_class_under(mRbPod, "Track", rb_cObject);

    rb_define_alloc_func(cRbPodTrack, rbpod_track_allocate);

    rb_define_method(cRbPodTrack, "initialize", rbpod_track_initialize, 0);

    sMEDIA_TYPE_AUDIO_VIDEO   = rb_intern("MEDIA_TYPE_AUDIO_VIDEO");
    sMEDIA_TYPE_AUDIO         = rb_intern("MEDIA_TYPE_AUDIO");
    sMEDIA_TYPE_VIDEO         = rb_intern("MEDIA_TYPE_VIDEO");
    sMEDIA_TYPE_PODCAST       = rb_intern("MEDIA_TYPE_PODCAST");
    sMEDIA_TYPE_VIDEO_PODCAST = rb_intern("MEDIA_TYPE_VIDEO_PODCAST");
    sMEDIA_TYPE_AUDIO_BOOK    = rb_intern("MEDIA_TYPE_AUDIO_BOOK");
    sMEDIA_TYPE_MUSIC_VIDEO   = rb_intern("MEDIA_TYPE_MUSIC_VIDEO");
    sMEDIA_TYPE_TV_SHOW_EXCL  = rb_intern("MEDIA_TYPE_TV_SHOW_EXCL");
    sMEDIA_TYPE_TV_SHOW       = rb_intern("MEDIA_TYPE_TV_SHOW");

    rb_const_set(cRbPodTrack, sMEDIA_TYPE_AUDIO_VIDEO,   UINT2NUM(MEDIA_TYPE_AUDIO_VIDEO));
    rb_const_set(cRbPodTrack, sMEDIA_TYPE_AUDIO,         UINT2NUM(MEDIA_TYPE_AUDIO));
    rb_const_set(cRbPodTrack, sMEDIA_TYPE_VIDEO,         UINT2NUM(MEDIA_TYPE_VIDEO));
    rb_const_set(cRbPodTrack, sMEDIA_TYPE_PODCAST,       UINT2NUM(MEDIA_TYPE_PODCAST));
    rb_const_set(cRbPodTrack, sMEDIA_TYPE_VIDEO_PODCAST, UINT2NUM(MEDIA_TYPE_VIDEO_PODCAST));
    rb_const_set(cRbPodTrack, sMEDIA_TYPE_AUDIO_BOOK,    UINT2NUM(MEDIA_TYPE_AUDIO_BOOK));
    rb_const_set(cRbPodTrack, sMEDIA_TYPE_MUSIC_VIDEO,   UINT2NUM(MEDIA_TYPE_MUSIC_VIDEO));
    rb_const_set(cRbPodTrack, sMEDIA_TYPE_TV_SHOW_EXCL,  UINT2NUM(MEDIA_TYPE_TV_SHOW_EXCL));
    rb_const_set(cRbPodTrack, sMEDIA_TYPE_TV_SHOW,       UINT2NUM(MEDIA_TYPE_TV_SHOW));

    DCL_ATTR(cRbPodTrack, track, title,            title);
    DCL_ATTR(cRbPodTrack, track, ipod_path,        ipod_path);
    DCL_ATTR(cRbPodTrack, track, album,            album);
    DCL_ATTR(cRbPodTrack, track, artist,           artist);
    DCL_ATTR(cRbPodTrack, track, genre,            genre);
    DCL_ATTR(cRbPodTrack, track, file_type,        filetype);
    DCL_ATTR(cRbPodTrack, track, comment,          comment);
    DCL_ATTR(cRbPodTrack, track, category,         category);
    DCL_ATTR(cRbPodTrack, track, composer,         composer);
    DCL_ATTR(cRbPodTrack, track, grouping,         grouping);
    DCL_ATTR(cRbPodTrack, track, description,      description);
    DCL_ATTR(cRbPodTrack, track, podcast_url,      podcasturl);
    DCL_ATTR(cRbPodTrack, track, podcast_rss,      podcastrss);
    DCL_ATTR(cRbPodTrack, track, subtitle,         subtitle);
    DCL_ATTR(cRbPodTrack, track, tv_show,          tvshow);
    DCL_ATTR(cRbPodTrack, track, tv_episode,       tvepisode);
    DCL_ATTR(cRbPodTrack, track, tv_network,       tvnetwork);
    DCL_ATTR(cRbPodTrack, track, album_artist,     albumartist);
    DCL_ATTR(cRbPodTrack, track, keywords,         keywords);
    DCL_ATTR(cRbPodTrack, track, sort_artist,      sort_artist);
    DCL_ATTR(cRbPodTrack, track, sort_title,       sort_title);
    DCL_ATTR(cRbPodTrack, track, sort_album,       sort_album);
    DCL_ATTR(cRbPodTrack, track, sort_albumartist, sort_albumartist);
    DCL_ATTR(cRbPodTrack, track, sort_composer,    sort_composer);
    DCL_ATTR(cRbPodTrack, track, sort_tvshow,      sort_tvshow);

    DCL_ATTR(cRbPodTrack, track, id,                id);
    DCL_ATTR(cRbPodTrack, track, size,              size);
    DCL_ATTR(cRbPodTrack, track, length,            tracklen);
    DCL_ATTR(cRbPodTrack, track, cd_number,         cd_nr);
    DCL_ATTR(cRbPodTrack, track, cd_total,          cds);
    DCL_ATTR(cRbPodTrack, track, track_number,      track_nr);
    DCL_ATTR(cRbPodTrack, track, track_total,       tracks);
    DCL_ATTR(cRbPodTrack, track, bitrate,           bitrate);
    DCL_ATTR(cRbPodTrack, track, samplerate,        samplerate);
    DCL_ATTR(cRbPodTrack, track, year,              year);
    DCL_ATTR(cRbPodTrack, track, volume,            volume);
    DCL_ATTR(cRbPodTrack, track, soundcheck,        soundcheck);
    DCL_ATTR(cRbPodTrack, track, bookmark,          bookmark_time);
    DCL_ATTR(cRbPodTrack, track, rating,            rating);
    DCL_ATTR(cRbPodTrack, track, play_count,        playcount);
    DCL_ATTR(cRbPodTrack, track, recent_play_count, recent_playcount);
    DCL_ATTR(cRbPodTrack, track, BPM,               BPM);
    DCL_ATTR(cRbPodTrack, track, start_offset,      starttime);
    DCL_ATTR(cRbPodTrack, track, stop_offset,       stoptime);
    DCL_ATTR(cRbPodTrack, track, skip_count,        skipcount);
    DCL_ATTR(cRbPodTrack, track, recent_skip_count, recent_skipcount);
    DCL_ATTR(cRbPodTrack, track, pregap,            pregap);
    DCL_ATTR(cRbPodTrack, track, sample_count,      samplecount);
    DCL_ATTR(cRbPodTrack, track, postgap,           postgap);
    DCL_ATTR(cRbPodTrack, track, media_type,        mediatype);
    DCL_ATTR(cRbPodTrack, track, season_number,     season_nr);
    DCL_ATTR(cRbPodTrack, track, episode_number,    episode_nr);

    DCL_ATTR_P(cRbPodTrack, track, transferred,         transferred);
    DCL_ATTR_P(cRbPodTrack, track, compilation,         compilation);
    DCL_ATTR_P(cRbPodTrack, track, checked,             checked);
    DCL_ATTR_P(cRbPodTrack, track, visible,             visible);
    DCL_ATTR_P(cRbPodTrack, track, explicit,            explicit_flag);
    DCL_ATTR_P(cRbPodTrack, track, has_artwork,         has_artwork);
    DCL_ATTR_P(cRbPodTrack, track, skip_when_shuffling, skip_when_shuffling);
    DCL_ATTR_P(cRbPodTrack, track, remember_position,   remember_playback_position);
    DCL_ATTR_P(cRbPodTrack, track, has_lyrics,          lyrics_flag);
    DCL_ATTR_P(cRbPodTrack, track, movie,               movie_flag);
    DCL_ATTR_P(cRbPodTrack, track, unplayed,            mark_unplayed);
    DCL_ATTR_P(cRbPodTrack, track, gapless,             gapless_track_flag);
    DCL_ATTR_P(cRbPodTrack, track, has_gapless_album,   gapless_album_flag);

    DCL_ATTR(cRbPodTrack, track, added_on,        time_added);
    DCL_ATTR(cRbPodTrack, track, modified_on,     time_modified);
    DCL_ATTR(cRbPodTrack, track, last_played_on,  time_played);
    DCL_ATTR(cRbPodTrack, track, released_on,     time_released);
    DCL_ATTR(cRbPodTrack, track, last_skipped_on, last_skipped);

    return;
}

