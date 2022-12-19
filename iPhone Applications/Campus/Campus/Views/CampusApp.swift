//
//  CampusApp.swift
//  Campus
//
//  Created by user224354 on 10/2/22.
//

import SwiftUI

@main
struct CampusApp: App {
    @StateObject var manager = MapManager()
    var body: some Scene {
        WindowGroup {
            MainCampusView()
                .environmentObject(manager)
        }
    }
}
