//
//  BeeQApp.swift
//  BeeQ
//
//  Created by user224354 on 8/24/22.
//

import SwiftUI
@main
struct BeeQApp: App {
    @StateObject var engine: BeeQEngine = BeeQEngine()
    var body: some Scene {
        WindowGroup {
            MainView()
                .environmentObject(engine)
        }
    }
}
