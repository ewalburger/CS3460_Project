#pragma once

enum class SortCriterion {
    PACKETS,
    BYTES
};

class ReportConfiguration {
public:
    ReportConfiguration(int interval, int number_to_show, SortCriterion sort_criterion);

    int interval() const;
    int numberToShow() const;
    SortCriterion sortCriterion() const;

private:
    int interval_;
    int number_to_show_;
    SortCriterion sort_criterion_;
};
