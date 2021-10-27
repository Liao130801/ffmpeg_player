package com.example.a6666;

public class MusicContent {
    String _data,_display_name,mime_type,title,artist,album;
    long _size,date_added,duration;

    public MusicContent(String _data, String _display_name, String mime_type, String title, String artist, String album, long _size, long date_added, long duration) {
        this._data = _data;
        this._display_name = _display_name;
        this.mime_type = mime_type;
        this.title = title;
        this.artist = artist;
        this.album = album;
        this._size = _size;
        this.date_added = date_added;
        this.duration = duration;
    }

    public String get_data() {
        return _data;
    }

    public void set_data(String _data) {
        this._data = _data;
    }

    public String get_display_name() {
        return _display_name;
    }

    public void set_display_name(String _display_name) {
        this._display_name = _display_name;
    }

    public String getMime_type() {
        return mime_type;
    }

    public void setMime_type(String mime_type) {
        this.mime_type = mime_type;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getArtist() {
        return artist;
    }

    public void setArtist(String artist) {
        this.artist = artist;
    }

    public String getAlbum() {
        return album;
    }

    public void setAlbum(String album) {
        this.album = album;
    }

    public long get_size() {
        return _size;
    }

    public void set_size(long _size) {
        this._size = _size;
    }

    public long getDate_added() {
        return date_added;
    }

    public void setDate_added(long date_added) {
        this.date_added = date_added;
    }

    public long getDuration() {
        return duration;
    }

    public void setDuration(long duration) {
        this.duration = duration;
    }
}
