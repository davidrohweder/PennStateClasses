//
//  PreferencesModel.swift
//  BeeQ
//
//  Created by user224354 on 9/6/22.
//

import Foundation

struct PreferencesModel  {
    static var language: languageType = .english
    static var characterOption: characterOptions = .five
    var preferences: [PForm]
    
    enum languageType: String, CaseIterable, Identifiable {
        case english, french
        var id: RawValue { rawValue }
    }
    
    enum characterOptions: String, CaseIterable, Identifiable {
        case five, six, seven
        var id: RawValue { rawValue }
    }
    
    init () {
        preferences = [PForm(uid: 0, titleString: "Character Options", formOptions: [0: "5",1: "6",2: "7"]), PForm(uid: 1, titleString: "Language Options", formOptions: [0: "English", 1: "French"])]
    }
    
}

struct PForm : Hashable, Equatable {
    var id: Int
    var title: String
    var options: [Int: String]
    var picked: Int = 0 {
        didSet {
            id == 0 ? setCharOpt() : setLanguage()
        }
    }
    
    init(uid: Int, titleString: String, formOptions: [Int: String]) {
        id = uid
        title = titleString
        options = formOptions
        picked = 0
    }
    
    func setCharOpt () -> Void {
        OneGame.characterOptions = Int(options[picked]!) ?? 0
    }
    
    func setLanguage () -> Void {
        PreferencesModel.language = options[picked] == "English" ? .english : .french
    }
    
    func hash(into hasher: inout Hasher) {
        hasher.combine(id)
    }
    
}
