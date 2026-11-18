#include "report_configuration.hpp"

#include <stdexcept>

ReportConfiguration::ReportConfiguration(
    int interval, int number_to_show, SortCriterion sort_criterion)
    : interval_(interval),
      number_to_show_(number_to_show),
      sort_criterion_(sort_criterion) {
    if (interval < 1 || interval > 2) {
        throw std::invalid_argument("Report interval must be 1 or 2 seconds.");
    }
    if (number_to_show < 0) {
        throw std::invalid_argument("Number of flows to show cannot be negative.");
    }
}

int ReportConfiguration::interval() const {
    return interval_;
}

int ReportConfiguration::numberToShow() const {
    return number_to_show_;
}

SortCriterion ReportConfiguration::sortCriterion() const {
    return sort_criterion_;
}
