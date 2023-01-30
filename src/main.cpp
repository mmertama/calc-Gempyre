#include <gempyre.h>
#include <gempyre_utils.h>
#include <cstdlib>
#include <tuple>
#include <cmath>
#include "computor.h"
#include "calc_resource.h"

std::string trim(const std::string& s) {
    const auto f = s.find_first_not_of('0');
    return f == std::string::npos ? "0" : s.substr(f);
}

Gempyre::Element::Elements getClass(Gempyre::Ui& ui, const std::string& name) {
    const auto els = ui.by_class(name);
    gempyre_utils_assert_x(els.has_value(), "Cannot get " + name);
    const auto v = els.value();
    gempyre_utils_assert_x(!v.empty(), "Cannot find " + name);
    return v;
}

int main(int /*argc*/, char** /*argv*/) {
    // GempyreUtils::setLogLevel(GempyreUtils::LogLevel::Debug);
    Computor::Computor comp;

    Gempyre::Ui ui(Calc_resourceh, "calc.html", "Calculator", 280, 320,  Gempyre::Ui::NoResize);
    //Gempyre::Ui ui(Calc_resourceh, "calc.html", GempyreUtils::htmlFileLaunchCmd(), "");
    ui.set_logging(true);
    auto operation = [&comp](Gempyre::Element& screenEl, Gempyre::Element& resultEl, const std::string& op) mutable {
        const auto r = comp.push(op);
        screenEl.set_html(r.has_value() ? Computor::toString(r.value()) : "ERR");
        const auto m = comp.memory();
        resultEl.set_html(m.has_value() ? Computor::toString(m.value()) : "");
    };

    ui.on_open([&ui, &operation]() mutable {
        auto totalElement = getClass(ui, "total")[0];
        totalElement.set_html("0");
        const auto totalId = totalElement.id();
        const auto lastElement = getClass(ui, "last")[0];
        const auto lastId = lastElement.id();
        const auto calc_id = getClass(ui, "calculator")[0];
        std::function<void (const Gempyre::Element& )> getChildren;
        getChildren = [&ui, totalId, lastId, &operation, &getChildren] (const Gempyre::Element& ce) {
            auto children = ce.children();
            gempyre_utils_assert_x(children.has_value(), "Cannot find calculator elements");
            for(auto& el : children.value()) {
                GempyreUtils::log(GempyreUtils::LogLevel::Info, "el id", el.id());
                const auto atv = el.attributes();
                gempyre_utils_assert_x(atv.has_value(), "Cannot read calculator element attributes");
                const auto attrs = atv.value();
                GempyreUtils::log(GempyreUtils::LogLevel::Info, "attributes", el.id(),
                                  GempyreUtils::join<decltype (attrs), decltype (attrs)::value_type, std::string>(attrs, ",", [](const auto& p) {
                                                         return p.first + ":" + p.second;
                                                     }));
                const auto vit = attrs.find("class");
                if(vit != attrs.end()) {
                    const auto pos = vit->second.find("calc_");
                    if(pos != std::string::npos) {
                         const auto value = attrs.find("value");
                         gempyre_utils_assert_x(value != attrs.end(), "button has no value");
                         const auto key = value->second;
                         el.subscribe("click", [&ui, key, totalId, lastId, &operation] (const Gempyre::Event& ev) {
                            GempyreUtils::log(GempyreUtils::LogLevel::Info, lastId);
                            auto last = Gempyre::Element(ui, lastId);
                            auto total = Gempyre::Element(ui, totalId);
                            operation(total, last, key);
                         });
                     }
                 }
                getChildren(el); // recursive
            }
        };
        getChildren(calc_id);
        /*The given UI size may not be correct for all platorms so we can ask after
        render and resize the UI as a workaround*/
        const auto ce_rect = ui.root().rect();//ce.rect();
        if(ce_rect) {
            ui.resize(ce_rect->width, ce_rect->height);
        } else {
            GempyreUtils::log(GempyreUtils::LogLevel::Error, "rect N/A");
        }
    });
    ui.run();
    return 0;
}

/*
 *   Some test code kept
    std::vector<std::string> input = {"0", "0", "1", "2", "0", ".", ".", "3", "4", "+", "2", "1", "=", Computor::Sqrt, "3", "9", Computor::Exp2, ".", Computor::Denom, "9", Computor::Denom, "6", Computor::Exp, "="};
   Computor::Computor comp;
   for(const auto& i : input) {
       const auto r =  comp.push(i);
       Utils::log(Utils::LogLevel::Info, "Add", i, "out is", r.has_value() ? toString(r.value()) : "ERR");
   }
    return 1;
*/
